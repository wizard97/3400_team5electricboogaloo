#include <VGAGraphics.h>

#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF


VGAGraphics lcd;

void setup() {
  // put your setup code here, to run once:
  lcd.init();
  lcd.clear();
  
  int lwidth = 3; //line width
  int bwidth = (lcd.width()-5*lwidth)/5; //box width
  int idx = bwidth/2;
  int llength = (bwidth+lwidth)*4;

  int y_off = (lcd.height()-llength)/2;
  for (int i=0; i < 5; i++) {
    lcd.fillRect(idx, y_off, lwidth, llength, CYAN);
    idx += lwidth+bwidth;
  }

  int idy = bwidth/2;
   for (int i=0; i < 4; i++) {
    lcd.fillRect(0, y_off+idy, lcd.width(), lwidth, CYAN);
    idy += lwidth+bwidth;
  }

  
  
}

void loop() {


}
 
    

