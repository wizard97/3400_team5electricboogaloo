#ifndef BOOGALOO_VGA_GRAPHICS_H
#define BOOGALOO_VGA_GRAPHICS_H

#include "Adafruit_GFX.h"
#include <Arduino.h>
#include <SPI.h>

#define VGA_SS 10

#if defined (__AVR__) || (__avr__)
  #define ENABLE_CS() do { DDRB |= B00000100; } while(0)
  #define CS_HIGH() do { PORTB |= B00000100; } while(0)
  #define CS_LOW() do { PORTB &= ~B00000100; } while(0)
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


};

#endif
