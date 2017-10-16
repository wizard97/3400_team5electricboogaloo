#include <SPI.h>

#define VGA_SS 10
#define XRES 64
#define YRES 64

// define pins for buttons to move block arround
#define BUTTON_UP 3
#define BUTTON_DOWN 4
#define BUTTON_LEFT 5
#define BUTTON_RIGHT 6

#define BLOCK_COLOR (genColor(7,0,0))

void clearScreen(uint8_t color=0);
uint8_t genColor(uint8_t r, uint8_t g, uint8_t b);
void writePixel(uint8_t x, uint8_t y, uint8_t color);

void setup() 
{
  Serial.begin(115200);
  // set the slaveSelectPin as an output:
  pinMode(VGA_SS, OUTPUT);
  digitalWrite(VGA_SS, HIGH);
  // initialize SPI:
  SPI.begin();

  //init buttons
  pinMode(BUTTON_UP, OUTPUT);
  pinMode(BUTTON_DOWN, OUTPUT);
  pinMode(BUTTON_LEFT, OUTPUT);
  pinMode(BUTTON_RIGHT, OUTPUT);
}

void loop() 
{ 
  static uint8_t b_x = 32;
  static uint8_t b_y = 32;
  static uint8_t color = BLOCK_COLOR;

  uint8_t bup = digitalRead(BUTTON_UP);
  uint8_t bdown = digitalRead(BUTTON_DOWN);
  uint8_t bleft = digitalRead(BUTTON_LEFT);
  uint8_t bright = digitalRead(BUTTON_RIGHT);

  if (bup || bdown || bleft || bright) {
    if (bup && b_y < YRES) {
       b_x +=1;
    } if (bdown && b_y != 0) {
      b_y -= 1;
    } if (bleft && b_x != 0) {
      b_x -= 1;
    } if (bright && b_x < XRES) {
      b_x += 1;
    }
    clearScreen();
    writePixel(b_x, b_y, color);
  }


  // Change block color
  while (Serial.available()) {
    char c = Serial.read();

    switch (c) {
      case 'r':
        color = genColor(7,0,0);
        break;

       case 'g':
        color = genColor(0,7,0);
        break;

        case 'b':
        color = genColor(0,0,3);
        break;
    }
  }
  
  

  delay(100);
}


/* Use this to write to FPGA vga driver
 * x cord: [0:63]
 * y cord: [0:63] 
 * color: Red: color[7:5], Green: color[4:2], Blue: blue[1:0]
 */
void writePixel(uint8_t x, uint8_t y, uint8_t color) 
{
  digitalWrite(VGA_SS, LOW);
  uint8_t data[] = {x, y, color};
  SPI.transfer(data, 3);
  digitalWrite(VGA_SS, HIGH);
}



void clearScreen(uint8_t color)
{
  for (uint8_t i=0; i < XRES; i++) {
    for (uint8_t j=0; j < YRES; j++) {
      writePixel(i, j, color);
    }
  }
}

// r: [0,7], g: [0,7], b: [0,3]
uint8_t genColor(uint8_t r, uint8_t g, uint8_t b)
{
   r &= 0b111;
   g &= 0b111;
   b &= 0b11;

   return (r << 4) | (g << 2) | b;
}

