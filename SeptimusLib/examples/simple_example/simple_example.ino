/*
 * An example of raw communication with the Septimus
 * 7 segment display.
 *
 * (c) 2014, Robin Scheibler/fakufaku
 * This example is released in the public domain.
 */

#include <Wire.h>

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

#define CMD_01_SEGMENTS 0x01
#define CMD_02_BRIGHTNESS 0x02
#define CMD_03_RATE 0x03

#define SLAVE_ADDRESS 50
#define BASE 10
unsigned int counter = 0;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
}

void loop()
{
  unsigned int t = counter;

  // send via two wire protocol
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(CMD_01_SEGMENTS);
  for (int i = 0 ; i < 4 ; i++)
  {
    Wire.write(numbertable[t%BASE]);
    t /= BASE;
  }
  Wire.endTransmission();

  // increment and delay
  counter++;
  delay(10);

}
