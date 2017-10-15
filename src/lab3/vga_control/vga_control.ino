#include <SPI.h>

#define VGA_SS 10


void setup() 
{
  // set the slaveSelectPin as an output:
  pinMode(VGA_SS, OUTPUT);
  digitalWrite(VGA_SS, HIGH);
  // initialize SPI:
  SPI.begin();
}

void loop() 
{ 
  // Write random values to each pixel
  for (uint8_t i=0; i < 64; i++) {
    for (uint8_t j=0; j < 64; j++) {
      uint8_t color = random(0, 255);
      writePixel(i, j, color);
    }
  }

  delay(1000);
}


/* Use this to write to FPGA vga driver
 * x cord: [0:63]
 * y cord: [0:63] 
 * color: Red: color[7:5], Green: color[4:2], Blue: blue[1:0]
 */
void writePixel(uint8_t x, uint8_t y, uint8_t color) {
  digitalWrite(VGA_SS, LOW);
  uint8_t data[] = {x, y, color};
  SPI.transfer(data, 3);
  digitalWrite(VGA_SS, HIGH);
}
