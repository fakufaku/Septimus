/*************************************************** 
  This is a library for our I2C LED Backpacks

  Designed specifically to work with the Adafruit LED 7-Segment backpacks 
  ----> http://www.adafruit.com/products/881
  ----> http://www.adafruit.com/products/880
  ----> http://www.adafruit.com/products/879
  ----> http://www.adafruit.com/products/878

  These displays use I2C to communicate, 2 pins are required to 
  interface. There are multiple selectable I2C addresses. For backpacks
  with 2 Address Select pins: 0x70, 0x71, 0x72 or 0x73. For backpacks
  with 3 Address Select pins: 0x70 thru 0x77

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  Modified by Robin Scheibler/fakufaku for Septimus big 7 segment display
  BSD license, all text above must be included in any redistribution
 ****************************************************/

// Enable one of these two #includes and comment out the other.
// Conditional #include doesn't work due to Arduino IDE shenanigans.
#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
//#include <TinyWireM.h> // Enable this line if using Adafruit Trinket, Gemma, etc.

#include <Septimus.h>

Septimus display = Septimus(SLAVE_ADDRESS);

void setup() {
#ifndef __AVR_ATtiny85__
  Serial.begin(9600);
  Serial.println("7 Segment Backpack Test");
#endif
  display.begin();

  // clear display
  display.clear();
  display.writeDisplay();
}

void loop() {
  // try to print a number thats too long
  display.print(10000, DEC);
  display.writeDisplay();
  delay(3000);

  // print a hex number
  display.print(0xBEEF, HEX);
  display.writeDisplay();
  delay(3000);

  // print a floating point 
  /* this isnt working yet, still hacking on it!
  display.print(12.34);
  display.writeDisplay();
  delay(500);
  */
  
  // print with print/println
  for (uint16_t counter = 0; counter < 9999; counter++) 
  {
    int c = counter % 510;
    if (c > 255) c = 510 - c;
    display.setBrightness(c);

    display.println(counter);
    display.writeDisplay();
    delay(10);
  }

  // max brightness
  display.setBrightness(255);

  // method #2 - draw each digit
  uint16_t blinkcounter = 0;
  boolean drawDots = false;
  for (uint16_t counter = 0; counter < 9999; counter ++) 
  {
    display.writeDigitNum(0, (counter / 1000), drawDots);
    display.writeDigitNum(1, (counter / 100) % 10, drawDots);
    display.writeDigitNum(2, (counter / 10) % 10, drawDots);
    display.writeDigitNum(3, counter % 10, drawDots);
   
    blinkcounter+=50;
    if (blinkcounter < 500) 
    {
      drawDots = false;
    } 
    else if (blinkcounter < 1000) 
    {
      drawDots = true;
    } 
    else 
    {
      blinkcounter = 0;
    }
    display.writeDisplay();
    delay(10);
  }
}
