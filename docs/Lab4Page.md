# __*Lab 4*__

### Objective
The objective of this lab was to first implement radio communication between two Arduinos and then integrate this functionality with the VGA component from Lab 3 to draw a complete maze on a VGA monitor and update the maze by transmitting information from one Arduino "radio" to the other.

### Work Distribution
As in the previous labs, we split into two sub-teams for this lab; Kelsey, Rajiv, and Christina worked on the radio component, and Aaron, David, and Adam worked on the FPGA component.

### Helpful Links
* [RF24 Class Reference](http://maniacbug.github.io/RF24/classRF24.html)

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
For setup, we first connected the two Nordic transceivers to each of our Arduinos in the correct SPI pins. After that we downloaded the specified RF24 Arduino library into the Arduino folder. As an extra note, we also had to move a printf.h function from a nested folder out into the main RF24 folder so that the example code, GettingStarted, could run. From here we were able to start working on our goals for lab 4.

##### Implementation
To implement the radio transmission, there were approximately two segments of code we had to configure: the transmit code and the receive code. Provided in the initial code downloaded from the sample library was a skeleton for the transmission and receive protocols. In the initial code, all it did was send a garbage value and verify that it was received by displaying “ok” on the serial monitor. It was very basic, however provided a rigid structure for adding more complex functionality.

[Here's a demonstration of the communication between the two Arduinos!](https://www.youtube.com/watch?v=0cg3QgOLpR0)

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

##### Drawing the Maze

Our first step was to transmit our own unique values. This amounted to sending a custom maze, similar in format to the one sent by Team Alpha in their demo. We did this by initializing a char maze as a global variable, ensuring that the maze data was less than the default data packet size, 32-bytes. Our 4x5 maze of chars, as you can see, is significantly less than this, so there was no errors in this regard.

In the transmit code, we merely had to change a single line of code, so that now it would transmit the maze structure via the radio.write() function. On the receive side as well, we merely had to save this data into a separate local variable got_maze which we then displayed to the serial monitor via a for-loop that iterated through the individual positions in the maze and displayed the data.

Once we ensured that we could configure the transmit and receive code segments to run unique values, we worked on sending varying data packets that were encoded with specific information about certain x and y positions in the maze and state values for those positions. Similar to the example, we encoded all this information into a single byte. We used 2 bits for the x position, 3 bits for the y position, and 2 bits for the state value. In our demo, we only considered one value for the state position: visited.

The format for our data is as follows:


x position | y position | state

This came out to seven bits, which we then padded with zeros to the default data packet size of 32 bytes, and sent it across the radio using radio.write(). To parse the data into its constituent parts, we used the following three lines of code.
/*KELSEYadd the following from our code:
unsigned char p = got_data & 0b11;
        unsigned char x = got_data >> 5 & 0b11;
        unsigned char y = got_data >> 2 & 0b111;
**
This allowed us to then update our maze with specific values for x and y as well as the state value. We displayed the maze in a similar manner as before and were able to update values for position on the fly during the demo.

The rest of the receive code is shown below:

*KELSEY//rest of the receive code***

#### FPGA Team

##### Setup

##### Implementation
To draw the maze we took advantage of the Adafruit GTX library. We created a white rectangle first to represented the entire poster board and then ontop of that we placed a 4 X 5 grid of lines to represent the black tape. We then placed a red rectangle ontop to denote the edge of the board.
![alt text](Lab4pics/code_snippet_maze_creation.jpg)

See [here](https://www.youtube.com/watch?v=G8H8aAYuhPc) for a video demonstration of our extended display!

In [this](https://www.youtube.com/watch?v=aIYxe3nmHjY) video, you can see a demonstration of how we transmitted randomly generated coordinates from the transmitter radio to the receiver radio and then used this data to update our maze grid.
