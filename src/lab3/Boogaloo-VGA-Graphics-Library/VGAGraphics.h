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


};

#endif
