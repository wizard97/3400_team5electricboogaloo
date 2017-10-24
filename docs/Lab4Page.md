# __*Lab 4*__

### Objective
The objective of this lab was to first implement radio communication between two Arduinos and then integrate this functionality with the VGA component from Lab 3 to draw a complete maze on a VGA monitor and update the maze by transmitting information from one Arduino "radio" to the other.

### Work Distribution
As in the previous labs, we split into two sub-teams for this lab; Kelsey, Rajiv, and Christina worked on the radio component, and Aaron, David, and Adam worked on the FPGA component.

### Helpful Links
* [RF24 Class Reference](http://maniacbug.github.io/RF24/classRF24.html)
* [Adafruit GFX Library](https://learn.adafruit.com/adafruit-gfx-graphics-library)

### Lab Documentation

#### Materials
* 2 Nordic nRF24L01+ transceivers
* 2 Arduino Unos
* 2 radio breakout boards
* FPGA
* 1 VGA cable
* 1 VGA connector
* 1 VGA switch
* Resistors

#### Radio Team

##### Setup

##### Implementation
[Here's a demonstration of the communication between the two Arduinos!](https://www.youtube.com/watch?v=0cg3QgOLpR0)

#### FPGA Team
##### FPGA Communication Protocol Modifications
The FPGA code was for the most part unchanged from the previous lab. The only change was updating the resolution sent over SPI from 120x120 to 240x240, which would
be mapped to a 480x480 box in the middle of the display. Our SPI protocol was also unchanged from the previous lab, the first byte sent is the x-coordinate (0-240), the second the y-coordinate (0-240), and the last byte the color.

To increase the draw speed of the Arduino, the SPI clock divider was changed from 4 MHz to 8 MHz with:
```c++
SPI.setClockDivider(SPI_CLOCK_DIV2); //faster 8mhz
```

Additionally, to speed up the drawing speed, instead of using `digitalWrite()` for SPI chip select, direct port manipulation (if available) was used instead:
```c++
#if defined (__AVR__) || (__avr__)
  #define ENABLE_CS() do { DDRB |= B00000010; } while(0)
  #define CS_HIGH() do { PORTB |= B00000010; } while(0)
  #define CS_LOW() do { PORTB &= ~B00000010; } while(0)
#else
  #define ENABLE_CS() do { pinMode(VGA_SS, OUTPUT); } while(0)
  #define CS_HIGH() do { digitalWrite(VGA_SS, HIGH); } while(0)
  #define CS_LOW() do { digitalWrite(VGA_SS, LOW); } while(0)
#endif
```

After both these performance improvements, the entire 240x240 display could be updated in roughly a second.


##### Extending the Adafruit GFX Library
Now that it was easy to draw pixels directly from the microcontroller, the hard part was mapping objects we wanted to draw to actual pixels to set on the screen.
The easy solution we came up with was inheriting from the [Adafruit GFX Library](https://learn.adafruit.com/adafruit-gfx-graphics-library) which is an open source library for drawing graphics with microcontrollers. The library provides an `Adafruit_GFX` class that is an abstract base class with a `virtual void drawPixel(int16_t x, int16_t y, uint16_t color)` method. By inheriting from this class and overriding the `drawPixel()` method to hook into our FPGA SPI interface. Our new derived class
has access to easy drawing primitives defined in the `Adafruit_GFX` base class that include methods for drawing lines, rectangles, text, and bitmaps. Some small additional conversions were needed to be compatible with the `Adafruit_GFX` Library, such as converting our 8-bit color representation to the libraries 16-bit representation and vice-versa.

Our inherited C++ class is defined as follows, notice the override of the `drawPixel()` method as well as some additional helper methods such as drawing an 8-bit color bitmap:

```c++
#ifndef BOOGALOO_VGA_GRAPHICS_H
#define BOOGALOO_VGA_GRAPHICS_H

#include "Adafruit_GFX.h"
#include <Arduino.h>
#include <SPI.h>

#define VGA_SS 9

#if defined (__AVR__) || (__avr__)
  #define ENABLE_CS() do { DDRB |= B00000010; } while(0)
  #define CS_HIGH() do { PORTB |= B00000010; } while(0)
  #define CS_LOW() do { PORTB &= ~B00000010; } while(0)
#else
  #define ENABLE_CS() do { pinMode(VGA_SS, OUTPUT); } while(0)
  #define CS_HIGH() do { digitalWrite(VGA_SS, HIGH); } while(0)
  #define CS_LOW() do { digitalWrite(VGA_SS, LOW); } while(0)
#endif


class VGAGraphics : public Adafruit_GFX
{
public:
  VGAGraphics(uint16_t x=240, uint16_t y=240)
  : Adafruit_GFX(x, y)
  {
    ENABLE_CS();
    CS_HIGH();
  }


  // defer Spi.begin
  void init()
  {
    CS_HIGH();

    SPI.begin();
#if defined (__AVR__) || (__avr__)
    SPI.setClockDivider(SPI_CLOCK_DIV2); //faster 8mhz
#endif
  }


  virtual void drawPixel(int16_t x, int16_t y, uint16_t color) override
  {
    if (x >= width() || y >= height()) // Out of range
      return;
    uint8_t c_red = (color >> 13) & 0x7;
    uint8_t c_green = (color >> 8) & 0x7;
    uint8_t c_blue = (color >> 3) & 0x3;

    uint8_t cnew = (c_red << 5) | (c_green << 2) | c_blue;
    uint8_t data[] = {x, y, cnew};
    CS_LOW();
    SPI.transfer(data, 3);
    CS_HIGH();
  }

  // Draw a PROGMEM-resident 8-bit image (RGB 3/3/2) at the specified (x,y)
// position.  For 16-bit display devices; no color reduction performed.
void drawRGB8Bitmap(int16_t x, int16_t y,
  const uint8_t bitmap[], int16_t w, int16_t h)
 {
    startWrite();
    for(int16_t j=0; j<h; j++, y++) {
        for(int16_t i=0; i<w; i++ ) {
            uint8_t cbm = pgm_read_byte(&bitmap[j * w + i]);
            uint8_t c_red = (cbm & 0xe0) >> 5;
            uint8_t c_green = (cbm & 0x1c) >> 2;
            uint8_t c_blue = (cbm & 0x03);
            uint16_t color = c_red << 13 | c_green << 8 | c_blue << 3;
            writePixel(x+i, y, color);
        }
    }
    endWrite();
}

void clear()
{
    fillScreen(0);
}


};

#endif
```


##### Setup

##### Implementation
To draw the maze we took advantage of the Adafruit GTX library. We created a white rectangle first to represented the entire poster board and then ontop of that we placed a 4 X 5 grid of lines to represent the black tape. We then placed a red rectangle ontop to denote the edge of the board.
![alt text](Lab4pics/code_snippet_maze_creation.jpg)

See [here](https://www.youtube.com/watch?v=G8H8aAYuhPc) for a video demonstration of our extended display!

In [this](https://www.youtube.com/watch?v=aIYxe3nmHjY) video, you can see a demonstration of how we transmitted randomly generated coordinates from the transmitter radio to the receiver radio and then used this data to update our maze grid.
