/*************************************************** 
  This is a example sketch demonstrating the graphics
  capabilities of the SSD1331 library  for the 0.96" 
  16-bit Color OLED with SSD1331 driver chip

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/684

  These ds use SPI to communicate, 4 or 5 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

const int FADERS = 4;
const byte DApins[] = {26,27,28};

// You can use any (4 or) 5 pins 
//#define sclk 52
//#define mosi 51
#define cs   53
#define rst  48
#define dc   49

// Color definitions
#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0  
#define WHITE           0xFFFF

#include <Adafruit_GFX.h>
#include "cscashby_SSD1331.h"
#include <SPI.h>

// Option 1: use any pins but a little slower
//Adafruit_SSD1331 d = Adafruit_SSD1331(cs, dc, mosi, sclk, rst);  

// Option 2: must use the hardware SPI pins 
// (for UNO thats sclk = 13 and sid = 11) and pin 10 must be 
// an output. This is much faster - also required if you want
// to use the microSD card (see the image drawing example)
cscashby_SSD1331 d = cscashby_SSD1331(cs, dc, rst);

float p = 3.1415926;

void setup(void) {
  Serial.begin(9600);
  Serial.println("START");
  
  for( int bt=0; bt<sizeof(DApins); bt++ ) {
    pinMode(DApins[bt], OUTPUT);
    Serial.print("pin ");
    Serial.print(DApins[bt]);
    Serial.print("=OUTPUT,");
    digitalWrite(DApins[bt], LOW);
  }

  Serial.println();

  d.init();
  d.begin();

  Serial.println("init");
  uint16_t time = millis();
  d.fillScreen(BLACK);
  time = millis() - time;
  
  Serial.println(time, DEC);
  delay(500);
   
  lcdTestPattern();
  delay(1000);
  
  d.fillScreen(BLACK);
  d.setCursor(0,0);
  d.print("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur adipiscing ante sed nibh tincidunt feugiat. Maecenas enim massa");
  delay(1000);

  // tft print function!
  tftPrintTest();
  delay(2000);
  
  //a single pixel
  d.drawPixel(d.width()/2, d.height()/2, GREEN);
  delay(500);
  
  // line draw test
  testlines(YELLOW);
  delay(500);    
  
  // optimized lines
  testfastlines(RED, BLUE);
  delay(500);    
 
  testdrawrects(GREEN);
  delay(1000);

  testfillrects(YELLOW, MAGENTA);
  delay(1000);

  d.fillScreen(BLACK);
  testfillcircles(10, BLUE);
  testdrawcircles(10, WHITE);
  delay(1000);

  testroundrects();
  delay(500);
  
  testtriangles();
  delay(500);
  
  Serial.println("done");
  delay(1000);
}

void loop() {
}

void testlines(uint16_t color) {
   d.fillScreen(BLACK);
   for (int16_t x=0; x < d.width()-1; x+=6) {
     d.drawLine(0, 0, x, d.height()-1, color);
   }
   for (int16_t y=0; y < d.height()-1; y+=6) {
     d.drawLine(0, 0, d.width()-1, y, color);
   }
   
   d.fillScreen(BLACK);
   for (int16_t x=0; x < d.width()-1; x+=6) {
     d.drawLine(d.width()-1, 0, x, d.height()-1, color);
   }
   for (int16_t y=0; y < d.height()-1; y+=6) {
     d.drawLine(d.width()-1, 0, 0, y, color);
   }
   
   d.fillScreen(BLACK);
   for (int16_t x=0; x < d.width()-1; x+=6) {
     d.drawLine(0, d.height()-1, x, 0, color);
   }
   for (int16_t y=0; y < d.height()-1; y+=6) {
     d.drawLine(0, d.height()-1, d.width()-1, y, color);
   }

   d.fillScreen(BLACK);
   for (int16_t x=0; x < d.width()-1; x+=6) {
     d.drawLine(d.width()-1, d.height()-1, x, 0, color);
   }
   for (int16_t y=0; y < d.height()-1; y+=6) {
     d.drawLine(d.width()-1, d.height()-1, 0, y, color);
   }
   
}

void testdrawtext(char *text, uint16_t color) {
  d.setTextSize(1);
  d.setTextColor(WHITE);
  d.setCursor(0,0);

  for (uint8_t i=0; i < 168; i++) {
    if (i == '\n') continue;
    d.write(i);
    if ((i > 0) && (i % 21 == 0))
      d.println();
  }    
}

void testfastlines(uint16_t color1, uint16_t color2) {
   d.fillScreen(BLACK);
   for (int16_t y=0; y < d.height()-1; y+=5) {
     d.drawFastHLine(0, y, d.width()-1, color1);
   }
   for (int16_t x=0; x < d.width()-1; x+=5) {
     d.drawFastVLine(x, 0, d.height()-1, color2);
   }
}

void testdrawrects(uint16_t color) {
 d.fillScreen(BLACK);
 for (int16_t x=0; x < d.height()-1; x+=6) {
   d.drawRect((d.width()-1)/2 -x/2, (d.height()-1)/2 -x/2 , x, x, color);
 }
}

void testfillrects(uint16_t color1, uint16_t color2) {
 d.fillScreen(BLACK);
 for (int16_t x=d.height()-1; x > 6; x-=6) {
   d.fillRect((d.width()-1)/2 -x/2, (d.height()-1)/2 -x/2 , x, x, color1);
   d.drawRect((d.width()-1)/2 -x/2, (d.height()-1)/2 -x/2 , x, x, color2);
 }
}

void testfillcircles(uint8_t radius, uint16_t color) {
  for (uint8_t x=radius; x < d.width()-1; x+=radius*2) {
    for (uint8_t y=radius; y < d.height()-1; y+=radius*2) {
      d.fillCircle(x, y, radius, color);
    }
  }  
}

void testdrawcircles(uint8_t radius, uint16_t color) {
  for (int16_t x=0; x < d.width()-1+radius; x+=radius*2) {
    for (int16_t y=0; y < d.height()-1+radius; y+=radius*2) {
      d.drawCircle(x, y, radius, color);
    }
  }  
}

void testtriangles() {
  d.fillScreen(BLACK);
  int color = 0xF800;
  int t;
  int w = d.width()/2;
  int x = d.height();
  int y = 0;
  int z = d.width();
  for(t = 0 ; t <= 15; t+=1) {
    d.drawTriangle(w, y, y, x, z, x, color);
    x-=4;
    y+=4;
    z-=4;
    color+=100;
  }
}

void testroundrects() {
  d.fillScreen(BLACK);
  int color = 100;
  int i;
  int t;
  for(t = 0 ; t <= 4; t+=1) {
  int x = 0;
  int y = 0;
  int w = d.width();
  int h = d.height();
    for(i = 0 ; i <= 24; i+=1) {
    d.drawRoundRect(x, y, w, h, 5, color);
    x+=2;
    y+=3;
    w-=4;
    h-=6;
    color+=1100;
  }
  color+=100;
  }
}

void tftPrintTest() {
  d.fillScreen(BLACK);
  d.setCursor(0, 5);
  d.setTextColor(RED);  
  d.setTextSize(1);
  d.println("Hello World!");
  d.setTextColor(YELLOW, GREEN);
  d.setTextSize(2);
  d.print("Hello Wo");
  d.setTextColor(BLUE);
  d.setTextSize(3);
  d.print(1234.567);
  delay(1500);
  d.setCursor(0, 5);
  d.fillScreen(BLACK);
  d.setTextColor(WHITE);
  d.setTextSize(0);
  d.println("Hello World!");
  d.setTextSize(1);
  d.setTextColor(GREEN);
  d.print(p, 5);
  d.println(" Want pi?");
  d.print(8675309, HEX); // print 8,675,309 out in HEX!
  d.print(" Print HEX");
  d.setTextColor(WHITE);
  d.println("Sketch has been");
  d.println("running for: ");
  d.setTextColor(MAGENTA);
  d.print(millis() / 1000);
  d.setTextColor(WHITE);
  d.print(" seconds.");
}

void mediabuttons() {
 // play
  d.fillScreen(BLACK);
  d.fillRoundRect(25, 10, 78, 60, 8, WHITE);
  d.fillTriangle(42, 20, 42, 60, 90, 40, RED);
  delay(500);
  // pause
  d.fillRoundRect(25, 90, 78, 60, 8, WHITE);
  d.fillRoundRect(39, 98, 20, 45, 5, GREEN);
  d.fillRoundRect(69, 98, 20, 45, 5, GREEN);
  delay(500);
  // play color
  d.fillTriangle(42, 20, 42, 60, 90, 40, BLUE);
  delay(50);
  // pause color
  d.fillRoundRect(39, 98, 20, 45, 5, RED);
  d.fillRoundRect(69, 98, 20, 45, 5, RED);
  // play color
  d.fillTriangle(42, 20, 42, 60, 90, 40, GREEN);
}

/**************************************************************************/
/*! 
    @brief  Renders a simple test pattern on the LCD
*/
/**************************************************************************/
void lcdTestPattern(void)
{
  uint32_t i,j;
  d.goTo(0, 0);
  
  for(i=0;i<64;i++)
  {
    for(j=0;j<96;j++)
    {
      if(i>55){d.writeData(WHITE>>8);d.writeData(WHITE);}
      else if(i>47){d.writeData(BLUE>>8);d.writeData(BLUE);}
      else if(i>39){d.writeData(GREEN>>8);d.writeData(GREEN);}
      else if(i>31){d.writeData(CYAN>>8);d.writeData(CYAN);}
      else if(i>23){d.writeData(RED>>8);d.writeData(RED);}
      else if(i>15){d.writeData(MAGENTA>>8);d.writeData(MAGENTA);}
      else if(i>7){d.writeData(YELLOW>>8);d.writeData(YELLOW);}
      else {d.writeData(BLACK>>8);d.writeData(BLACK);}
    }
  }
}
