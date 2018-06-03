// You can use any (4 or) 5 pins 
#define sclk 52
#define mosi 51
#define cs1  53
#define cs2   4
#define rst1  3
#define rst2  5
#define dc   11

int sensorPin = A0;    // select the input pin for the potentiometer
int sensorValue = 0;

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
#include <Adafruit_SSD1331.h>
#include <Adafruit_HX8357.h>
#include <SPI.h>

// Option 1: use any pins but a little slower
//Adafruit_SSD1331 display1 = Adafruit_SSD1331(cs1, dc, mosi, sclk, rst1);  
//Adafruit_SSD1331 display2 = Adafruit_SSD1331(cs2, dc, mosi, sclk, rst2);  

// Option 2: must use the hardware SPI pins 
// (for UNO thats sclk = 13 and sid = 11) and pin 10 must be 
// an output. This is much faster - also required if you want
// to use the microSD card (see the image drawing example)
Adafruit_SSD1331 display1 = Adafruit_SSD1331(cs1, dc, rst1);
Adafruit_SSD1331 display2 = Adafruit_SSD1331(cs2, dc, rst2);

// These are 'flexible' lines that can be changed
#define TFT_CS 41
#define TFT_DC 36
#define TFT_RST 37 // RST can be set to -1 if you tie it to Arduino's reset

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC, TFT_RST);

float p = 3.1415926;

void setup(void) {
  Serial.begin(9600);
  Serial.print("hello!");
  display1.begin();
  display2.begin();

  Serial.println("init");
  uint16_t time = millis();
  display1.fillScreen(BLACK);
  display1.setCursor(0,0);
  display2.fillScreen(BLACK);
  display2.setCursor(0,0);

  tft.begin(HX8357D);

  // read diagnostics (optional but can help debug problems)
  uint8_t x = tft.readcommand8(HX8357_RDPOWMODE);
  Serial.print("Display Power Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(HX8357_RDMADCTL);
  Serial.print("MADCTL Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(HX8357_RDCOLMOD);
  Serial.print("Pixel Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(HX8357_RDDIM);
  Serial.print("Image Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(HX8357_RDDSDR);
  Serial.print("Self Diagnostic: 0x"); Serial.println(x, HEX); 
  
  Serial.println(F("Benchmark                Time (microseconds)"));

  tft.setRotation(1);
}

void loop() {
  sensorValue = analogRead(sensorPin);
  
  display1.setCursor(0, 5);
  display1.setTextColor(BLUE,BLACK);  
  display1.setTextSize(1);
  display1.println("Hello World1!");
  display1.print("Sensor: ");
  display1.print(sensorValue);
  display1.println("   ");

  display2.setCursor(0, 5);
  display2.setTextColor(WHITE,BLACK);  
  display2.setTextSize(1);
  display2.println("Hello World2!");
  display2.print("Sensor: ");
  display2.print(sensorValue);
  display2.println("   ");

  display1.fillRect((display1.width()-1), 0, 1, display1.height(), BLACK);
  display1.fillRect((display1.width()-1), ((float)sensorValue / 1023) * display1.height(), 1, display1.height() - (((float)sensorValue / 1023) * display1.height()), BLUE);

  display2.fillRect((display2.width()-1), 0, 1, display2.height(), BLACK);
  display2.fillRect((display2.width()-1), ((float)sensorValue / 1023) * display2.height(), 1, display2.height() - (((float)sensorValue / 1023) * display2.height()), RED);
}

void testlines(uint16_t color) {
   display2.fillScreen(BLACK);
   for (int16_t x=0; x < display2.width()-1; x+=6) {
     display2.drawLine(0, 0, x, display2.height()-1, color);
   }
   for (int16_t y=0; y < display2.height()-1; y+=6) {
     display2.drawLine(0, 0, display2.width()-1, y, color);
   }
   
   display2.fillScreen(BLACK);
   for (int16_t x=0; x < display2.width()-1; x+=6) {
     display2.drawLine(display2.width()-1, 0, x, display2.height()-1, color);
   }
   for (int16_t y=0; y < display2.height()-1; y+=6) {
     display2.drawLine(display2.width()-1, 0, 0, y, color);
   }
   
   display2.fillScreen(BLACK);
   for (int16_t x=0; x < display2.width()-1; x+=6) {
     display2.drawLine(0, display2.height()-1, x, 0, color);
   }
   for (int16_t y=0; y < display2.height()-1; y+=6) {
     display2.drawLine(0, display2.height()-1, display2.width()-1, y, color);
   }

   display2.fillScreen(BLACK);
   for (int16_t x=0; x < display2.width()-1; x+=6) {
     display2.drawLine(display2.width()-1, display2.height()-1, x, 0, color);
   }
   for (int16_t y=0; y < display2.height()-1; y+=6) {
     display2.drawLine(display2.width()-1, display2.height()-1, 0, y, color);
   }
   
}

void testdrawtext(char *text, uint16_t color) {
  display2.setTextSize(1);
  display2.setTextColor(WHITE);
  display2.setCursor(0,0);

  for (uint8_t i=0; i < 168; i++) {
    if (i == '\n') continue;
    display2.write(i);
    if ((i > 0) && (i % 21 == 0))
      display2.println();
  }    
}

void testfastlines(uint16_t color1, uint16_t color2) {
   display2.fillScreen(BLACK);
   for (int16_t y=0; y < display2.height()-1; y+=5) {
     display2.drawFastHLine(0, y, display2.width()-1, color1);
   }
   for (int16_t x=0; x < display2.width()-1; x+=5) {
     display2.drawFastVLine(x, 0, display2.height()-1, color2);
   }
}

void testdrawrects(uint16_t color) {
 display2.fillScreen(BLACK);
 for (int16_t x=0; x < display2.height()-1; x+=6) {
   display2.drawRect((display2.width()-1)/2 -x/2, (display2.height()-1)/2 -x/2 , x, x, color);
 }
}

void testfillrects(uint16_t color1, uint16_t color2) {
 display2.fillScreen(BLACK);
 for (int16_t x=display2.height()-1; x > 6; x-=6) {
   display2.fillRect((display2.width()-1)/2 -x/2, (display2.height()-1)/2 -x/2 , x, x, color1);
   display2.drawRect((display2.width()-1)/2 -x/2, (display2.height()-1)/2 -x/2 , x, x, color2);
 }
}

void testfillcircles(uint8_t radius, uint16_t color) {
  for (uint8_t x=radius; x < display2.width()-1; x+=radius*2) {
    for (uint8_t y=radius; y < display2.height()-1; y+=radius*2) {
      display2.fillCircle(x, y, radius, color);
    }
  }  
}

void testdrawcircles(uint8_t radius, uint16_t color) {
  for (int16_t x=0; x < display2.width()-1+radius; x+=radius*2) {
    for (int16_t y=0; y < display2.height()-1+radius; y+=radius*2) {
      display2.drawCircle(x, y, radius, color);
    }
  }  
}

void testtriangles() {
  display2.fillScreen(BLACK);
  int color = 0xF800;
  int t;
  int w = display2.width()/2;
  int x = display2.height();
  int y = 0;
  int z = display2.width();
  for(t = 0 ; t <= 15; t+=1) {
    display2.drawTriangle(w, y, y, x, z, x, color);
    x-=4;
    y+=4;
    z-=4;
    color+=100;
  }
}

void testroundrects() {
  display2.fillScreen(BLACK);
  int color = 100;
  int i;
  int t;
  for(t = 0 ; t <= 4; t+=1) {
  int x = 0;
  int y = 0;
  int w = display2.width();
  int h = display2.height();
    for(i = 0 ; i <= 24; i+=1) {
    display2.drawRoundRect(x, y, w, h, 5, color);
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
  display1.fillScreen(BLACK);
  display1.setCursor(0, 5);
  display1.setTextColor(RED);  
  display1.setTextSize(1);
  display1.println("Hello World!");
  display1.setTextColor(YELLOW, GREEN);
  display1.setTextSize(2);
  display1.print("Hello Wo");
  display1.setTextColor(BLUE);
  display1.setTextSize(3);
  display1.print(1234.567);
  delay(1500);
  display1.setCursor(0, 5);
  display1.fillScreen(BLACK);
  display1.setTextColor(WHITE);
  display1.setTextSize(0);
  display1.println("Hello World!");
  display1.setTextSize(1);
  display1.setTextColor(GREEN);
  display1.print(p, 5);
  display1.println(" Want pi?");
  display1.print(8675309, HEX); // print 8,675,309 out in HEX!
  display1.print(" Print HEX");
  display1.setTextColor(WHITE);
  display1.println("Sketch has been");
  display1.println("running for: ");
  display1.setTextColor(MAGENTA);
  display1.print(millis() / 1000);
  display1.setTextColor(WHITE);
  display1.print(" seconds.");
}

void mediabuttons() {
 // play
  display2.fillScreen(BLACK);
  display2.fillRoundRect(25, 10, 78, 60, 8, WHITE);
  display2.fillTriangle(42, 20, 42, 60, 90, 40, RED);
  delay(500);
  // pause
  display2.fillRoundRect(25, 90, 78, 60, 8, WHITE);
  display2.fillRoundRect(39, 98, 20, 45, 5, GREEN);
  display2.fillRoundRect(69, 98, 20, 45, 5, GREEN);
  delay(500);
  // play color
  display2.fillTriangle(42, 20, 42, 60, 90, 40, BLUE);
  delay(50);
  // pause color
  display2.fillRoundRect(39, 98, 20, 45, 5, RED);
  display2.fillRoundRect(69, 98, 20, 45, 5, RED);
  // play color
  display2.fillTriangle(42, 20, 42, 60, 90, 40, GREEN);
}

/**************************************************************************/
/*! 
    @brief  Renders a simple test pattern on the LCD
*/
/**************************************************************************/
void lcdTestPattern(void)
{
  uint32_t i,j;
  display2.goTo(0, 0);
  
  for(i=0;i<64;i++)
  {
    for(j=0;j<96;j++)
    {
      if(i>55){display2.writeData(WHITE>>8);display2.writeData(WHITE);}
      else if(i>47){display2.writeData(BLUE>>8);display2.writeData(BLUE);}
      else if(i>39){display2.writeData(GREEN>>8);display2.writeData(GREEN);}
      else if(i>31){display2.writeData(CYAN>>8);display2.writeData(CYAN);}
      else if(i>23){display2.writeData(RED>>8);display2.writeData(RED);}
      else if(i>15){display2.writeData(MAGENTA>>8);display2.writeData(MAGENTA);}
      else if(i>7){display2.writeData(YELLOW>>8);display2.writeData(YELLOW);}
      else {display2.writeData(BLACK>>8);display2.writeData(BLACK);}
    }
  }
}
