#include <VGAGraphics.h>

#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define DIMX 5
#define DIMY 4
#define LWIDTH 3


VGAGraphics lcd;

// we use entire length of X
// 240x240

void setup() {
  // put your setup code here, to run once:
  lcd.init();
  lcd.clear();

  int bwidth = (lcd.width()-DIMX*LWIDTH)/DIMX; //box width
  int llengthx = (bwidth+LWIDTH)*DIMX; //grid line length in X
  int llengthy = (bwidth+LWIDTH)*DIMY; //grid line length in Y
  int y_off = (lcd.height()-llengthy)/2;

  // draw vertical bars
  int idx = bwidth/2;
  for (int i=0; i < DIMX; i++) {
    lcd.fillRect(idx, y_off, LWIDTH, llengthy, CYAN); //widht, height
    idx += LWIDTH+bwidth;
  }

  // draw horz
  int idy = bwidth/2;
   for (int i=0; i < DIMY; i++) {
    lcd.fillRect(0, y_off+idy, llengthx, LWIDTH, CYAN);
    idy += LWIDTH+bwidth;
  }
  

  // y boundary
  lcd.fillRect(0, y_off, llengthx, LWIDTH, BLUE);
  lcd.fillRect(0, y_off + llengthy-LWIDTH, llengthx, LWIDTH, BLUE);

  //x boundary
  lcd.fillRect(0, y_off, LWIDTH, llengthy, BLUE);
  lcd.fillRect(llengthx-LWIDTH, y_off, LWIDTH, llengthy, BLUE);

  
  
}

void loop() {


}
 
    

