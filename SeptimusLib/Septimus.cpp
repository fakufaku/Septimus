/***************************************************
 * This is the library for I2C big 7 segment display 'Septimus'
 *
 * This library was adapted from adafruit's i2c matrix led backpack
 * library. It was slimmed down and adapted to work with this
 * custom 7 segment back pack 'Septimus'
 * ----> https://github.com/fakufaku/Septimus
 *
 * Septimus uses I2C to communicate, 2 pins are required to interface. 
 * The I2C address of the display is 50 (decimal)
 *
 * Originally written by Limor Fried/Ladyada for Adafruit Industries.
 * Adapted for Septimus by Robin Scheibler/fakufaku
 * BSD license, all text above must be included in any redistribution
 * ****************************************************/

#ifdef __AVR_ATtiny85__
#include <TinyWireM.h>
#define Wire TinyWireM
#else
#include <Wire.h>
#endif

#include "Septimus.h"

static const uint8_t numbertable[] = {
  0x3F, /* 0 */
  0x06, /* 1 */
  0x5B, /* 2 */
  0x4F, /* 3 */
  0x66, /* 4 */
  0x6D, /* 5 */
  0x7D, /* 6 */
  0x07, /* 7 */
  0x7F, /* 8 */
  0x6F, /* 9 */
  0x77, /* a */
  0x7C, /* b */
  0x39, /* C */
  0x5E, /* d */
  0x79, /* E */
  0x71, /* F */
};

void Septimus::setBrightness(uint8_t b) 
{
  Wire.beginTransmission(i2c_addr);
  Wire.write(CMD_02_BRIGHTNESS);
  Wire.write(b);
  Wire.endTransmission();
}

Septimus::Septimus(uint8_t _addr = SLAVE_ADDRESS) 
{
  i2c_addr = _addr;
  position = 0;
}

void Septimus::begin(void)
{
  Wire.begin();
  setRate(RATE_488Hz);
  setBrightness(255);
}

void Septimus::writeDisplay(void) 
{
  Wire.beginTransmission(i2c_addr);
  Wire.write(CMD_01_SEGMENTS);
  for (uint8_t i = 0; i < DIGIT_NUMBER; i++)
  {
    Wire.write(displaybuffer[DIGIT_NUMBER-i-1]);
  }
  Wire.endTransmission();
}

void Septimus::setRate(uint8_t r)
{
  Wire.beginTransmission(i2c_addr);
  Wire.write(CMD_03_RATE);
  uint8_t v = Wire.write(r);
  Wire.endTransmission();
}

void Septimus::clear(void) 
{
  for (uint8_t i = 0; i < DIGIT_NUMBER; i++) {
    displaybuffer[i] = 0;
  }
}

void Septimus::print(unsigned long n, int base)
{
  if (base == 0) write(n);
  else printNumber(n, base);
}

void Septimus::print(char c, int base)
{
  print((long) c, base);
}

void Septimus::print(unsigned char b, int base)
{
  print((unsigned long) b, base);
}

void Septimus::print(int n, int base)
{
  print((long) n, base);
}

void Septimus::print(unsigned int n, int base)
{
  print((unsigned long) n, base);
}

void Septimus::println(void) 
{
  position = 0;
}

void Septimus::println(char c, int base)
{
  print(c, base);
  println();
}

void Septimus::println(unsigned char b, int base)
{
  print(b, base);
  println();
}

void Septimus::println(int n, int base)
{
  print(n, base);
  println();
}

void Septimus::println(unsigned int n, int base)
{
  print(n, base);
  println();
}

void Septimus::println(long n, int base)
{
  print(n, base);
  println();
}

void Septimus::println(unsigned long n, int base)
{
  print(n, base);
  println();
}

void Septimus::println(double n, int digits)
{
  print(n, digits);
  println();
}

void Septimus::print(double n, int digits)
{
  printFloat(n, digits);
}


size_t Septimus::write(uint8_t c) 
{

  uint8_t r = 0;

  if (c == '\n') position = 0;
  if (c == '\r') position = 0;

  if ((c >= '0') && (c <= '9')) {
    writeDigitNum(position, c-'0');
    r = 1;
  }

  position++;

  return r;
}

void Septimus::writeDigitRaw(uint8_t d, uint8_t bitmask) 
{
  if (d >= 4) return;
  displaybuffer[d] = bitmask;
}

void Septimus::writeDigitNum(uint8_t d, uint8_t num, boolean dot) 
{
  if (d >= 4) return;

  writeDigitRaw(d, numbertable[num] | (dot << 7));
}

void Septimus::print(long n, int base)
{
  printNumber(n, base);
}

void Septimus::printNumber(long n, uint8_t base)
{
  printFloat(n, 0, base);
}

void Septimus::printFloat(double n, uint8_t fracDigits, uint8_t base)
{
  uint8_t numericDigits = 4; // available digits on display
  boolean isNegative = false; // true if the number is negative

  // is the number negative?
  if(n < 0) 
  {
    isNegative = true; // need to draw sign later
    --numericDigits; // the sign will take up one digit
    n *= -1; // pretend the number is positive
  }

  // calculate the factor required to shift all fractional digits
  // into the integer part of the number
  double toIntFactor = 1.0;
  for(int i = 0; i < fracDigits; ++i) toIntFactor *= base;

  // create integer containing digits to display by applying
  // shifting factor and rounding adjustment
  uint32_t displayNumber = n * toIntFactor + 0.5;

  // calculate upper bound on displayNumber given
  // available digits on display
  uint32_t tooBig = 1;
  for(int i = 0; i < numericDigits; ++i) tooBig *= base;

  // if displayNumber is too large, try fewer fractional digits
  while(displayNumber >= tooBig) 
  {
    --fracDigits;
    toIntFactor /= base;
    displayNumber = n * toIntFactor + 0.5;
  }

  // did toIntFactor shift the decimal off the display?
  if (toIntFactor < 1) 
  {
    printError();
  } 
  else 
  {
    // otherwise, display the number
    int8_t displayPos = 3;

    if (displayNumber) //if displayNumber is not 0
    {
      for(uint8_t i = 0; displayNumber; ++i) 
      {
        boolean displayDecimal = (fracDigits != 0 && i == fracDigits);
        writeDigitNum(displayPos--, displayNumber % base, displayDecimal);
        displayNumber /= base;
      }
    }
    else 
    {
      writeDigitNum(displayPos--, 0, false);
    }

    // display negative sign if negative
    if(isNegative) writeDigitRaw(displayPos--, 0x40);

    // clear remaining display positions
    while(displayPos >= 0) writeDigitRaw(displayPos--, 0x00);
  }
}

void Septimus::printError(void) {
  uint8_t E = numbertable[14]; // upper case 'E'
  uint8_t r = 0x50; // lower case 'r'
  writeDigitRaw(0, E); // 'E'
  writeDigitRaw(1, r); // 'r'
  writeDigitRaw(2, r | 0x80);  // 'r.'
  writeDigitRaw(3, 0); // nothing
}
