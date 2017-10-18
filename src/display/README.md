The `Boogaloo-VGA-Graphics-Library` inherits from the Adafruit's `Adafruit-GFX-Library-master` and overrides the drawPixel interface into
our SPI VGA FPGA controller. This gives us access to all of Adafruit's GFX lib's methods, see the documentation [here](https://learn.adafruit.com/adafruit-gfx-graphics-library/overview).

We can now easily draw colored lines, text, rectangles, circles, bitmaps, etc.. All without touching the FPGA firmware.

Our screen resolution is 240x240.
