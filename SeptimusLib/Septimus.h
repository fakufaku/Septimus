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

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#ifdef __AVR_ATtiny85__
#include <TinyWireM.h>
#else
#include <Wire.h>
#endif

#define DIGIT_NUMBER 4
#define SLAVE_ADDRESS 50

// The list of commands recognized by the display
#define CMD_01_SEGMENTS 0x01
#define CMD_02_BRIGHTNESS 0x02
#define CMD_03_RATE 0x03

#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2
#define BYTE 0

#define RATE_STOP 0x0
#define RATE_4kHz 0x1
#define RATE_488Hz 0x2
#define RATE_61Hz 0x3

class Septimus {
  public:
    Septimus(uint8_t _addr);
    void begin(void);
    void setBrightness(uint8_t b);
    void writeDisplay(void);
    void setRate(uint8_t r);
    void clear(void);

    uint8_t displaybuffer[DIGIT_NUMBER];

    size_t write(uint8_t c);

    void print(char, int = BYTE);
    void print(unsigned char, int = BYTE);
    void print(int, int = DEC);
    void print(unsigned int, int = DEC);
    void print(long, int = DEC);
    void print(unsigned long, int = DEC);
    void print(double, int = 2);
    void println(char, int = BYTE);
    void println(unsigned char, int = BYTE);
    void println(int, int = DEC);
    void println(unsigned int, int = DEC);
    void println(long, int = DEC);
    void println(unsigned long, int = DEC);
    void println(double, int = 2);
    void println(void);

    void writeDigitRaw(uint8_t x, uint8_t bitmask);
    void writeDigitNum(uint8_t x, uint8_t num, boolean dot = false);
    void printNumber(long, uint8_t = 2);
    void printFloat(double, uint8_t = 2, uint8_t = DEC);
    void printError(void);

  private:
    uint8_t i2c_addr;
    uint8_t position;
};
