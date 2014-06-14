Septimus
========

[Septimus](https://en.wikipedia.org/wiki/Septimus_%28praenomen%29) is a driver
board to control large (2.3 inches) seven segments displays. It is designed for four
characters and has its own step-up converter to produce the large voltage
needed by the large seven segments displays.

The display is controlled by an ATtiny2313. The display is controlled through
the two-wire interface of the microcontroller.

All the components are commonly available discrete through-hole parts.

<img src="https://raw.githubusercontent.com/fakufaku/Septimus/master/board.jpg" width=1024>

Because of the size of the 7-segments, I couldn't use the free version of Eagle
CAD and decided to take the chance to learn its free alternative, the [gEDA suite](http://geda-project.org/).  
I found this [tutorial](http://wiki.geda-project.org/geda:gsch2pcb_tutorial)
particularly useful in my learning process. The combo of gschem/pcb is quite
powerful and has a simplicity that Eagle CAD lacks. However, the workflow to go
from schematics to PCB and keep consistency is very cumbersome and error prone.
Making custom parts is also not very practical. As a conclusion, it was a good
experience and is definitely an advantage to know an EDA without the
limitations of Eagle CAD with respect to board size and multiple layers, but I
will stick to Eagle for my simple projects.

Bill of material
----------------

* ATtiny2313 (DIP20)
* NJM2360AD Voltage regulator (drop-in replacement of [MC34063A](http://www.ti.com/litv/slls636m))
* 6x NPN transistors (C1815, ECB) (Q101, Q201, Q203, Q205, Q207, Q209)
* 4x PNP transistors (A1015, ECB) (Q202, Q204, Q206, Q208)
* 1x 1000uH coil (300mil pin spacing)
* 5x 1K resistor (R202, R206, R210, R214, R312)
* 7x 10K resistor (R201, R205, R209, R213, R301, R310, R311)
* 11x 270 resistor (R204, R208, R212, R126, R302, R303, R304, R305, R306, R307, R308)
* 4x 51 resistor (R203, R207, R211, R215)
* 3x 180 resistor (R101, R104, R309)
* 1x 9.1K resistor (R102)
* 1x 68K resistor (R103)
* 1x 10uF cap ceramic (C101)
* 1x 100uF cap electrolytic (C102)
* 1x 1nF cap ceramic (C103)
* 1x 0.1uF cap ceramic (C104)
* 1x diode (schottky, 40V)
* 1x ISP header (small footprint, 2x3 pins)
* 1x 1x4 straight pin header (100mil spacing)
* 4x 2.3inch 7-segments displays
* 1x 2.1mm barrel jack

Etch the board
--------------

The top and bottom mask files are `Septimus_mask_top.pdf` and `Septimus_mask_bot.pdf`,
respectively. The mask are mirrored so that the printed side goes directly onto the copper.
There are alignment marks and some text to avoid mistakes.

The board is double-sided and thus not trivial for home etching. However, there are only 
four tracks on the top layer and it is thus conceivable to make the PCB single layer
and manually patch it with four wires.

In addition, because of the difficulty of making metallized holes at home, a number
of jumper wires are required (7) as shown on `Septimus_info.pdf`. In the same file,
the drill bits sizes are described.

Alternatively, the gerber files are available in the folder `gerber` for sending
to a board house.

Firmware
--------

The firmware can be burnt using an ISP programmer (such as the usbtiny).

    cd firmware/
    make avrdude

If you do not use the usbtiny, edit the Makefile `PROGRAMMER` variable to
reflect the device you use.

The fuse settings are left to factory default (Low:0x64, High:0xDF, Ext:0xFF).

Arduino library
---------------

### Install

Copy `SeptimusLib/` into your local Arduino `libraries` folder. Restart
the Arduino IDE and try one of the examples.

### Example sketch.

    #include <Wire.h>
    #include <Septimus.h>

    Septimus display = Septimus(SLAVE_ADDRESS);

    unsigned int counter = 0;

    void setup() {
      display.begin();
    }

    void loop() {
      // print current counter value
      display.print(counter, DEC);
      display.writeDisplay();

      // increment and wait a second
      counter++;
      delay(1000);
    }

### Adjust brightness of display

It is possible to set the brightness of the display by using

    display.setBrightness(b)

where `b` is a value between 0 (dimmest) to 255 (brightest).

### Adjust blinking rate of display

The seven segment works by displaying each digit individually
but at high frequency so that we have the illusion of seeing
all digit lit up simultaneously. The blinking rate can be
changed using the following code. The same command can be used
to switch off the display.

    display.setRate(RATE_488Hz) // 488 Hz (default)
    display.setRate(RATE_4kHz)  // 4 kHz
    display.setRate(RATE_61Hz)  // 61 Hz
    display.setRate(RATE_STOP)  // display is off

Note that the only rate that gives smooth display and
allows for adjusting the brightness is 488 Hz, the default.

At 4kHz, the display is somewhat brighter, but only works
with brightness set to 255.

License
-------

### Schematics, PCB, firmware


(c) 2014, Robin Scheibler aka FakuFaku

    "THE BEER-WARE LICENSE" (Revision 42):
    <fakufaku@gmail.com> wrote this file. As long as you retain this notice you
    can do whatever you want with this stuff. If we meet some day, and you think
    this stuff is worth it, you can buy me a beer in return -- Robin Scheibler

![](https://upload.wikimedia.org/wikipedia/commons/d/d5/BeerWare_Logo.svg)

With the exception of the USI TWI Slave driver code by Donald R. Blake which is GPL.

### Arduino library

The arduino library was adapted from the excellent [Adafruit LED
Backpack](https://github.com/adafruit/Adafruit-LED-Backpack-Library) library
which is released under MIT or BSD license, depending on where you look.
