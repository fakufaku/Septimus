/*
* ----------------------------------------------------------------------------
* "THE BEER-WARE LICENSE" (Revision 42):
* <fakufaku@gmail.com> wrote this file. As long as you retain this notice you
* can do whatever you want with this stuff. If we meet some day, and you think
* this stuff is worth it, you can buy me a beer in return -- Robin Scheibler
* ----------------------------------------------------------------------------
*/

/*
 * This is the firmware of the Septimus big 7 segment display.
 * All code and schematics available online
 * https://github.com/fakufaku/Septimus
 */

#include <avr/sleep.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdlib.h>

#include "usiTwiSlave.h"

// Allows to disable interrupt in some parts of the code
#define ENTER_CRIT()    {char volatile saved_sreg = SREG; cli()
#define LEAVE_CRIT()    SREG = saved_sreg;}

// the different timer0 frequencies
#define TIMER0_CLKDIV_4kHz (1 << CS00)
#define TIMER0_CLKDIV_488Hz (1 << CS01)
#define TIMER0_CLKDIV_61Hz (1 << CS01) | (1 << CS00)
#define TIMER0_CLKDIV_15Hz (1 << CS02)
#define TIMER0_CLKDIV_4Hz (1 << CS02) | (1 << CS00)

// The different interrupts to enable
#define SET_OCR0A_INT() TIMSK |= (1 << OCIE0A)
#define UNSET_OCR0A_INT() TIMSK &= ~(1 << OCIE0A)
#define TIMER0_STOP() TCCR0B = 0
#define TIMER0_SET_RATE(R) TCCR0B = R

#define FIRMWARE_REVISION 1
#define SLAVE_ADDRESS 50

#define DIGIT_NUMBER 4

#define DEFAULT_BRIGHTNESS 0xFE

// the bit buffer
uint8_t buffer[4] = {0x0, 0x0, 0x0, 0x0};

// we use a timer to rotate the digits
uint8_t digit_counter = 0;

// brightness register
uint8_t brightness = DEFAULT_BRIGHTNESS;

// TWI commands
#define CMD_01_SEGMENTS 0x01
#define CMD_02_BRIGHTNESS 0x02
#define CMD_03_RATE 0x03

// define the digits pins
#define DIG4_PORT PORTD
#define DIG4_DDR  DDRD
#define DIG4_PIN  0

#define DIG3_PORT PORTD
#define DIG3_DDR  DDRD
#define DIG3_PIN  1

#define DIG2_PORT PORTA
#define DIG2_DDR  DDRA
#define DIG2_PIN  1

#define DIG1_PORT PORTA
#define DIG1_DDR  DDRA
#define DIG1_PIN  0

// define the segments pins
#define SEG_A_PORT PORTD
#define SEG_A_DDR  DDRD
#define SEG_A_PIN  3

#define SEG_B_PORT PORTD
#define SEG_B_DDR  DDRD
#define SEG_B_PIN  2

#define SEG_C_PORT PORTB
#define SEG_C_DDR  DDRB
#define SEG_C_PIN  2

#define SEG_D_PORT PORTB
#define SEG_D_DDR  DDRB
#define SEG_D_PIN  3

#define SEG_E_PORT PORTB
#define SEG_E_DDR  DDRB
#define SEG_E_PIN  4

#define SEG_F_PORT PORTB
#define SEG_F_DDR  DDRB
#define SEG_F_PIN  0

#define SEG_G_PORT PORTB
#define SEG_G_DDR  DDRB
#define SEG_G_PIN  1

#define SEG_DP_PORT PORTD
#define SEG_DP_DDR  DDRD
#define SEG_DP_PIN  4

// turn off all digits
void digits_all_off(void)
{
  DIG1_PORT &= ~_BV(DIG1_PIN);
  DIG2_PORT &= ~_BV(DIG2_PIN);
  DIG3_PORT &= ~_BV(DIG3_PIN);
  DIG4_PORT &= ~_BV(DIG4_PIN);
}

// light up a digit
void light_digit(uint8_t n)
{
  switch (n)
  {
    case 1:
      DIG1_PORT |= _BV(DIG1_PIN);
      break;

    case 2:
      DIG2_PORT |= _BV(DIG2_PIN);
      break;

    case 3:
      DIG3_PORT |= _BV(DIG3_PIN);
      break;

    case 4:
      DIG4_PORT |= _BV(DIG4_PIN);
      break;

    default:
      digits_all_off();
  }
}

// set the segments for a given number
void set_segments(uint8_t bits)
{
  //A
  if (bits & 0x1)
    SEG_A_DDR |= _BV(SEG_A_PIN);
  else
    SEG_A_DDR &= ~_BV(SEG_A_PIN);
  //B
  if (bits & 0x2)
    SEG_B_DDR |= _BV(SEG_B_PIN);
  else
    SEG_B_DDR &= ~_BV(SEG_B_PIN);
  //C
  if (bits & 0x4)
    SEG_C_DDR |= _BV(SEG_C_PIN);
  else
    SEG_C_DDR &= ~_BV(SEG_C_PIN);
  //D
  if (bits & 0x8)
    SEG_D_DDR |= _BV(SEG_D_PIN);
  else
    SEG_D_DDR &= ~_BV(SEG_D_PIN);
  //E
  if (bits & 0x10)
    SEG_E_DDR |= _BV(SEG_E_PIN);
  else
    SEG_E_DDR &= ~_BV(SEG_E_PIN);
  //F
  if (bits & 0x20)
    SEG_F_DDR |= _BV(SEG_F_PIN);
  else
    SEG_F_DDR &= ~_BV(SEG_F_PIN);
  //G
  if (bits & 0x40)
    SEG_G_DDR |= _BV(SEG_G_PIN);
  else
    SEG_G_DDR &= ~_BV(SEG_G_PIN);
  //DP
  if (bits & 0x80)
  {
    SEG_DP_DDR |= _BV(SEG_DP_PIN);
    SEG_DP_PORT |= _BV(SEG_DP_PIN);
  }
  else
  {
    SEG_DP_DDR |= _BV(SEG_DP_PIN);
    SEG_DP_PORT &= ~_BV(SEG_DP_PIN);
  }
}

// process the TWI command
void processTWI( void )
{
  uint8_t b,c;

  // receive the command
  b = usiTwiReceiveByte();

  switch (b) {
    case CMD_01_SEGMENTS: // save brightness
      // receive the 4 digits
      for (c = 0 ; c < DIGIT_NUMBER ; c++)
        buffer[c] = usiTwiReceiveByte();
      break;

    case CMD_02_BRIGHTNESS:
      // receive the brightness byte
      brightness = usiTwiReceiveByte();  // update the output compare A with new value
      if (brightness == 0xFF)
        UNSET_OCR0A_INT();
      else
        SET_OCR0A_INT();
      break;

    case CMD_03_RATE:
      c = usiTwiReceiveByte();
      switch (c)
      {
        case 0x00:
          TIMER0_STOP();
          break;

        case 0x01:
          TIMER0_SET_RATE(TIMER0_CLKDIV_4kHz);
          break;

        case 0x02:
          TIMER0_SET_RATE(TIMER0_CLKDIV_488Hz);
          break;

        case 0x03:
          TIMER0_SET_RATE(TIMER0_CLKDIV_61Hz);
          break;
      }
      break;

    default:
      // nothing to do
      break;
  }
}

// The output compare A interrupt to turn off the digit
SIGNAL(TIMER0_COMPA_vect)
{
  ENTER_CRIT();

  // turn off all digits
  digits_all_off();

  LEAVE_CRIT();
}

// The timer overflow interrupt routine
SIGNAL(TIMER0_OVF_vect)
{
  ENTER_CRIT();

  // turn off all digits
  digits_all_off();

  // increment the digit to display
  digit_counter = (digit_counter+1) % DIGIT_NUMBER;

  // set the segments
  set_segments(buffer[digit_counter]);

  // turn digit on
  light_digit(digit_counter+1);

  // set the time the digit should light up
  OCR0A = brightness;

  LEAVE_CRIT();
}

int main()
{
  // set digits as output and disable them all
  DIG1_DDR |= _BV(DIG1_PIN);
  DIG2_DDR |= _BV(DIG2_PIN);
  DIG3_DDR |= _BV(DIG3_PIN);
  DIG4_DDR |= _BV(DIG4_PIN);
  light_digit(0);

  // setup all segments as inputs and low
  SEG_A_DDR &= ~_BV(SEG_A_PIN);
  SEG_B_DDR &= ~_BV(SEG_B_PIN);
  SEG_C_DDR &= ~_BV(SEG_C_PIN);
  SEG_D_DDR &= ~_BV(SEG_D_PIN);
  SEG_E_DDR &= ~_BV(SEG_E_PIN);
  SEG_F_DDR &= ~_BV(SEG_F_PIN);
  SEG_G_DDR &= ~_BV(SEG_G_PIN);
  
  SEG_A_PORT &= ~_BV(SEG_A_PIN);
  SEG_B_PORT &= ~_BV(SEG_B_PIN);
  SEG_C_PORT &= ~_BV(SEG_C_PIN);
  SEG_D_PORT &= ~_BV(SEG_D_PIN);
  SEG_E_PORT &= ~_BV(SEG_E_PIN);
  SEG_F_PORT &= ~_BV(SEG_F_PIN);
  SEG_G_PORT &= ~_BV(SEG_G_PIN);

  // set DP as output and to ground
  SEG_DP_DDR |= _BV(SEG_DP_PIN);
  SEG_DP_PORT &= ~_BV(SEG_DP_PIN);

  // set up timer 1 as system clock
  TIMSK = (1 << TOIE0);         // overflow interrupts
  TCCR0B = TIMER0_CLKDIV_488Hz; // default blink rate at 488Hz

  
  // init the TWI slave
  usiTwiSlaveInit(SLAVE_ADDRESS);

  // enable interrupts
  sei();

  // The infinite loop
  while (1)
  {
    // process Twi commands
    while (usiTwiDataInReceiveBuffer())
    {
      processTWI();
    }
  }

}




