// General libraries
#include <SPI.h>

// Libraries for displays
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1331.h>

#define DISPLAY_FADERS  4

#define DISPLAY_CS    53
#define DISPLAY_RST   48
#define DISPLAY_DC    49

const byte displayAddressPins[] = {26,27,28};

// Display color definitions
#define BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0  
#define WHITE           0xFFFF

Adafruit_SSD1331* displays[DISPLAY_FADERS];

void setup() {
  Serial.begin(9600);
  Serial.println("START");

  // Initialise displays
  // - set DA pins as output
  for( int bt=0; bt<sizeof(displayAddressPins); bt++ ) {
    pinMode(displayAddressPins[bt], OUTPUT);
  }
  for( byte fader = 0; fader < DISPLAY_FADERS; fader++ ) {
    for( int bt=0; bt<sizeof(displayAddressPins); bt++ ) {
      byte state = bitRead(fader, bt);
      digitalWrite(displayAddressPins[bt], state);
    }
    Adafruit_SSD1331 currentDisplay = Adafruit_SSD1331(DISPLAY_CS, DISPLAY_DC, DISPLAY_RST);
    displays[fader] = &currentDisplay;
    currentDisplay.begin();
    delay(200);
    currentDisplay.fillScreen(BLACK);
    delay(200);
    currentDisplay.setTextColor(BLUE);
    currentDisplay.setTextSize(3);
    currentDisplay.println(fader);
  }

  // Initialise faders
  // - calibration

  // Initialise LoRa radio stuff
}

void loop() {

}
