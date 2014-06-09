/*
* ----------------------------------------------------------------------------
* "THE BEER-WARE LICENSE" (Revision 42):
* <fakufaku@gmail.com> wrote this file. As long as you retain this notice you
* can do whatever you want with this stuff. If we meet some day, and you think
* this stuff is worth it, you can buy me a beer in return -- Robin Scheibler
* ----------------------------------------------------------------------------
*/

#include <avr/sleep.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// Allows to disable interrupt in some parts of the code
#define ENTER_CRIT()    {char volatile saved_sreg = SREG; cli()
#define LEAVE_CRIT()    SREG = saved_sreg;}

// The number displayed
unsigned long number = 0;
unsigned int dig = 1;

// short timer
#define SHORT_TIMER_LENGTH 4
unsigned char short_timer = 0;

// LED on/off array
unsigned char LED_array = 0;

// 7-segment mapping array
unsigned char numbers[11] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x00 };

// define the digits pins
#define DIG1_PORT PORTD
#define DIG1_DDR  DDRD
#define DIG1_PIN  0

#define DIG2_PORT PORTD
#define DIG2_DDR  DDRD
#define DIG2_PIN  1

#define DIG3_PORT PORTA
#define DIG3_DDR  DDRA
#define DIG3_PIN  1

#define DIG4_PORT PORTA
#define DIG4_DDR  DDRA
#define DIG4_PIN  0

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

// light up a digit
void light_digit(int n)
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
      DIG1_PORT &= ~_BV(DIG1_PIN);
      DIG2_PORT &= ~_BV(DIG2_PIN);
      DIG3_PORT &= ~_BV(DIG3_PIN);
      DIG4_PORT &= ~_BV(DIG4_PIN);
  }
}

// set the segments for a given number
void set_number(int n, int dp)
{
  unsigned char bits = 0;

  // convert number to bit pattern using table lookup
  if (n >= 0 && n <= 9)
    bits = numbers[n];
  else
    bits = 0;

  // set the decimal point if needed
  if (dp)
    bits |= 0x80;
  else
    bits &= ~0x80;

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
    //SEG_DP_DDR &= ~_BV(SEG_DP_PIN);
    SEG_DP_DDR |= _BV(SEG_DP_PIN);
    SEG_DP_PORT &= ~_BV(SEG_DP_PIN);
  }

}

// The timer overflow interrupt routine
SIGNAL(TIMER0_OVF_vect)
{
  short_timer = (short_timer + 1) % SHORT_TIMER_LENGTH;
  if (short_timer == 0)
  {
    // update the state
    /*
    number = (number + 1) % 10;
    dig = (dig + 1) % 4;
    light_digit(0);
    light_digit(dig+1);
    set_number(number, 0);
    */
    number = (number + 1) % 10000;
  }
}

int main()
{
  int i, j;

  // enable interrupts
  sei();

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
  SEG_DP_DDR &= ~_BV(SEG_DP_PIN);
  
  SEG_A_PORT &= ~_BV(SEG_A_PIN);
  SEG_B_PORT &= ~_BV(SEG_B_PIN);
  SEG_C_PORT &= ~_BV(SEG_C_PIN);
  SEG_D_PORT &= ~_BV(SEG_D_PIN);
  SEG_E_PORT &= ~_BV(SEG_E_PIN);
  SEG_F_PORT &= ~_BV(SEG_F_PIN);
  SEG_G_PORT &= ~_BV(SEG_G_PIN);
  SEG_DP_PORT &= ~_BV(SEG_DP_PIN);

  // set up timer 1 as system clock
  TCCR0B = (1 << CS02) | (1 << CS00); // T1 clock to clk/1024
  TIMSK = (1 << TOIE0);   // set the overflow interrupt

  // The infinite loop
  while (1)
  {
    unsigned long n = number;
    for (i = 4 ; i >= 1 ; i--)
    {
      light_digit(i);
      if (i == 2)
        set_number(n % 10, 1);
      else
        set_number(n % 10, 0);
      for (j = 0 ; j < 10 ; j++)
        ;
      n /= 10;
      set_number(10, 0);
      light_digit(0);
    }
  }

}




