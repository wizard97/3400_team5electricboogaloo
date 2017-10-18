The `Boogaloo-VGA-Graphics-Library` inherits from the Adafruit's `Adafruit-GFX-Library-master` and overrides the drawPixel interface hooking it into our SPI VGA FPGA controller. This gives us access to all of Adafruit's GFX lib's methods, see the documentation [here](https://learn.adafruit.com/adafruit-gfx-graphics-library/overview).

We can now easily draw colored lines, text, rectangles, circles, bitmaps, etc.. All without touching the FPGA firmware.

Our screen resolution is 240x240. Look at the `display_demo` which is a modified example for one of Adafruit's LCD's, that instead uses our inhereted library.

Look at the draw_kirstin demo to see how to draw bitmaps. The bitmap was generated with http://www.digole.com/tools/PicturetoC_Hex_converter.php.

Any of the public methods in this class are valid and you can use https://github.com/wizard97/3400_team5electricboogaloo/blob/master/src/display/Adafruit-GFX-Library-master/Adafruit_GFX.h

# Installing all the crap
Drop both the `Boogaloo-VGA-Graphics-Library` and `Adafruit-GFX-Library-master` folder into Documents/Arduino/libraries/ to install the
required stuff.
