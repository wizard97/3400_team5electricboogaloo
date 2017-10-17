#include <SPI.h>
#include <VGAGraphics.h>

#define VGA_SS 10

#define XRES 240
#define YRES 240

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
  //pinMode(VGA_SS, OUTPUT);
  DDRB |= B00000100; //set slave select output
  //digitalWrite(VGA_SS, HIGH);
  PORTB |= B00000100; // Set it high
  // initialize SPI:
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV2); //faster 8mhz

  //init buttons, configure pull up
  /*
  digitalWrite(BUTTON_UP, HIGH);
  digitalWrite(BUTTON_DOWN, HIGH);
  digitalWrite(BUTTON_LEFT, HIGH);
  digitalWrite(BUTTON_RIGHT, HIGH);
  */
  pinMode(BUTTON_UP, INPUT);
  pinMode(BUTTON_DOWN, INPUT);
  pinMode(BUTTON_LEFT, INPUT);
  pinMode(BUTTON_RIGHT, INPUT);

  clearScreen();
  
}
//64-36-
//64-48=16
void loop() 
{ 
  /*
  for (uint8_t i=0; i <= 5; i++) {
    for (uint8_t j=10; j < 120; j++)
      writePixel(10+20*i, j, 0xff);
  }


  for (uint8_t i=0; i <= 4; i++) {
    for (uint8_t j=0; j < 120; j++)
      writePixel(j, 20+20*i, 0xff);
  }
*/
/*
  for (uint8_t i=0; i <= 5; i++) {
    for (uint8_t j=0; j < 120; j++)
      writePixel(10+20*i, j, 0xff);
  }
  */
  writePixel(0, 0, 0xff);
  writePixel(119, 0, 0xff);
  writePixel(0, 119, 0xff);
  writePixel(119, 119, 0xff);
  //clearScreen();
  delay(5000);
}


/* Use this to write to FPGA vga driver
 * x cord: [0:63]
 * y cord: [0:63] 
 * color: Red: color[7:5], Green: color[4:2], Blue: blue[1:0]
 */
void writePixel(uint8_t x, uint8_t y, uint8_t color) 
{
  if (x >= XRES || y >= YRES)
    return;
    
  //digitalWrite(VGA_SS, LOW);
  PORTB &= ~B00000100; // This is faster!
  uint8_t data[] = {x, y, color};
  SPI.transfer(data, 3);
  //digitalWrite(VGA_SS, HIGH);
  PORTB |= B00000100;
}



void clearScreen(uint8_t color)
{
  for (uint8_t i=0; i < XRES; i++) {
    for (uint8_t j=0; j < YRES; j++) {
      writePixel(i, j, color);
    }
  }
}


void randScreen()
{
  for (uint8_t i=0; i < XRES; i++) {
    for (uint8_t j=0; j < YRES; j++) {
      writePixel(i, j, random(0, 255));
    }
  }
}

// r: [0,7], g: [0,7], b: [0,3]
uint8_t genColor(uint8_t r, uint8_t g, uint8_t b)
{
   r &= 0b111;
   g &= 0b111;
   b &= 0b11;

   return (r << 5) | (g << 2) | b;
}

