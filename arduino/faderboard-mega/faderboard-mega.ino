// General libraries
#include <SPI.h>

// Radio libraries
#include <RFM69.h>
//#include <RFM69_ATC.h>

// Libraries for displays
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1331.h>
#include <Adafruit_HX8357.h>

#include "MemoryFree.h"
#include "stringutils.h"

#define SERIAL_BAUD   115200

#define LOOP_WAIT     100

// Radio assignments
#define RADIO_CS      4
#define RADIO_RST     2
#define RADIO_INT     3
#define RADIO_BAND    RF69_433MHZ

// Uncomment if this board is the RFM69HW/HCW not the RFM69W/CW
#define RADIO_IS_RFM69HW_HCW

#define RADIO_MYID    2
#define RADIO_DESKID  1
#define RADIO_NETID   100

// Maximum number of times re try to send a batch of fader levels
#define RADIO_MAXRETRIES      5
// Current try count - -1 = not changed
int radio_sendtries = 0;
// Singleton RF69 object
RFM69 radio(RADIO_CS, RADIO_INT, false);

// Pin assignments
// - TODO: Make these one, but I'm testing with random numbers of things populated
#define DISPLAY_COUNT 4
#define FADER_COUNT   8

#define DISPLAY_CS    53
#define DISPLAY_RST   48
#define DISPLAY_DC    49
#define MOTOR_CTLA    5
#define MOTOR_CTLB    6
#define TFT_LITE      10
#define TFT_DC        11
#define TFT_CS        12
#define TFT_RST       13

// Singleton TFT object
Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC, TFT_RST);

// Need to check what's best for OSC here
#define FADER_NOTSENT 0
#define FADER_MIN     1
#define FADER_MAX     255

// Only send if fader changes by...
#define SEND_THRESHOLD 4

const byte displayAddressPins[] = {26,27,28};
const byte motorAddressPins[] = {7,8,9};
const byte faderAddressPins[] = {22,23,24};
const byte MApins[] = {7,8,9};

const byte FaderPins[] = {A0,A1,A2,A3};

// Constants
// - Time to wait for motor to move measurably
#define MOTOR_WAIT     20

#define MOTOR_FORWARD  1
#define MOTOR_BACKWARD 0

// - TFT rotation
#define TFT_ROTATION   3

// Uncomment this for faders to calibrate using motors
// - if commented, picks a guess (below) for max / min (quieter and quicker)
// - faders will calibrate first time they're used
//#define MOTOR_CALIBRATE

#define MOTOR_DEFAULTMIN  900
#define MOTOR_DEFAULTMAX  100

double fadersMin[FADER_COUNT];
double fadersMax[FADER_COUNT];
// Storage for fader values sent via radio
byte sentFaderVal[FADER_COUNT];

// Display color definitions
#define BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0  
#define WHITE           0xFFFF

Adafruit_SSD1331* displays[DISPLAY_COUNT];

void setup() {
  Serial.begin(SERIAL_BAUD);
  if( Serial ) Serial.println("START");

  // Initialise LoRa radio stuff
  initRadio();
  // Initialise fader motors
  initFaders();
  // Initialise displays (scribble strips)
  initDisplays();
  // Initialise TFT
  initTFT();
}

// Main loop
void loop() {
  Adafruit_SSD1331* currentDisplay;
  byte faderVal[FADER_COUNT];
  for( byte fader = 0; fader < FADER_COUNT; fader++ ) {
    faderVal[fader] = faderRead(fader);
    if( fader < DISPLAY_COUNT ) {
      currentDisplay = getDisplay(fader);
      currentDisplay->setTextSize(2);
      currentDisplay->setCursor(0,35);
      currentDisplay->setTextColor(RED, BLACK);
      currentDisplay->print(faderVal[fader]);
      currentDisplay->print("   ");
    }
    // If we're not in a retry loop and the fader values have changed then we need to send some data
    if( radio_sendtries == -1 && (faderVal[fader] < ( sentFaderVal[fader] - SEND_THRESHOLD ) || faderVal[fader] > ( sentFaderVal[fader] + SEND_THRESHOLD )) ) {
      radio_sendtries = 0;
    }
  }

  tft.setCursor(0, 20);
  for( byte fader = 0; fader < FADER_COUNT; fader++ ) {
    tft.setTextColor(HX8357_RED, HX8357_BLACK);
    tft.setTextSize(2);
    tft.print(u2s(faderVal[fader], 3));
    tft.print(" ");
  }

  if( radio_sendtries > -1 ) {
    if (Serial) Serial.print(radio_sendtries);
    if (Serial) Serial.print(" tries: Sending ");
    byte payload[FADER_COUNT];
    for( byte fader = 0; fader < FADER_COUNT; fader++ ) {
      payload[fader] = faderVal[fader];
      if (Serial) Serial.print(faderVal[fader]);
      if( Serial && fader - 1 < FADER_COUNT ) Serial.print(",");
    }
    if (radio.sendWithRetry(RADIO_DESKID, payload, sizeof(payload), 1, 400)) {
      if (Serial) Serial.println("... ACK received");
      for( byte fader = 0; fader < FADER_COUNT; fader++ ) {
        payload[fader] = faderVal[fader];
        // Register these values as sent
        sentFaderVal[fader] = faderVal[fader];
      }
      radio_sendtries = -1;
    } else {
      radio_sendtries++;
      if (Serial) Serial.print("... No ACK ");
      if (Serial) Serial.print(radio_sendtries);
      if (Serial) Serial.println(" retries");
      // We give up if we have tried too often, but we'll try again when the fader next changes
      if( radio_sendtries >= RADIO_MAXRETRIES ) radio_sendtries = -1;
      for( byte fader = 0; fader < FADER_COUNT; fader++ ) {
        sentFaderVal[fader] = faderVal[fader];
      }
    }
  }

  delay(LOOP_WAIT);
}

void initRadio() {
  // Reset the radio
  if( Serial ) Serial.print("Resetting radio...");
  pinMode(RADIO_RST, OUTPUT);
  digitalWrite(RADIO_RST, HIGH);
  delay(20);
  digitalWrite(RADIO_RST, LOW);
  delay(500);
  // Initialize the radio
  radio.initialize(RADIO_BAND, RADIO_MYID, RADIO_NETID);
  radio.promiscuous(true);
  #ifdef RADIO_IS_RFM69HW_HCW
    radio.setHighPower(); //must include this only for RFM69HW/HCW!
  #endif
  if( Serial ) Serial.println(" Radio ready");
}

void initDisplays() {
  if( Serial ) Serial.print("Display initialisation...");
  // - set DA pins as output
  setModeOfPins(displayAddressPins, OUTPUT);
  for( byte fader = 0; fader < DISPLAY_COUNT; fader++ ) {
    for( int bt=0; bt<=sizeof(displayAddressPins); bt++ ) {
      byte state = bitRead(fader, bt);
      digitalWrite(displayAddressPins[bt], state);
    }
    Adafruit_SSD1331* currentDisplay = new Adafruit_SSD1331(DISPLAY_CS, DISPLAY_DC, DISPLAY_RST);
    displays[fader] = currentDisplay;
    currentDisplay->begin();
    currentDisplay->fillScreen(BLACK);
    currentDisplay->setTextColor(BLUE);
    currentDisplay->setTextSize(3);
    currentDisplay->println(fader);
    currentDisplay->setTextSize(1);
    currentDisplay->print(fadersMin[fader]); currentDisplay->print(" - "); currentDisplay->print(fadersMax[fader]);
  }
  if( Serial ) Serial.println(" Displays ready");
}

Adafruit_SSD1331* getDisplay(byte fader) {
  for( int bt=0; bt<=sizeof(displayAddressPins); bt++ ) {
    byte state = bitRead(fader, bt);
    digitalWrite(displayAddressPins[bt], state);
  }
  return displays[fader];
}

void initTFT() {
  if( Serial ) Serial.print("TFT initialisation...");

  pinMode(TFT_LITE, OUTPUT);
  // TODO: Support PWM for LCD backlight
  digitalWrite(TFT_LITE, HIGH);

  tft.begin(HX8357D);
  tft.setRotation(TFT_ROTATION);
  
  tft.fillScreen(HX8357_BLACK);

  tft.setCursor(0, 0);
  tft.setTextColor(HX8357_WHITE);  tft.setTextSize(1);
  tft.println("Ready");

  if( Serial ) Serial.println(" TFT ready");
}

void initFaders() {
  if( Serial ) Serial.println("Motor initialisation...");
  pinMode(MOTOR_CTLA,OUTPUT);
  pinMode(MOTOR_CTLB,OUTPUT);
  setModeOfPins(motorAddressPins, OUTPUT);
  setModeOfPins(faderAddressPins, OUTPUT);
#ifdef MOTOR_CALIBRATE
  // - calibration
  //   - before / after fader variables to check if stopped moving
  double a,b;
  for( byte fader = 0; fader < FADER_COUNT; fader++ ) {
    sentFaderVal[fader] = FADER_NOTSENT;
    a = 0; b = 10;
    // - motor could be anywhere so go down first
    motorMove(fader, MOTOR_BACKWARD);
    while( a != b ) {
      a = faderReadRAW(fader);
      delay(MOTOR_WAIT);
      b = faderReadRAW(fader);
      //if( Serial ) Serial.print(fader); if( Serial ) Serial.print(" 1 "); if( Serial ) Serial.print(a); if( Serial ) Serial.print(","); if( Serial ) Serial.println(b);
    }
    a = 0; b = 10;
    motorMove(fader, MOTOR_FORWARD);
    while( a != b ) {
      a = faderReadRAW(fader);
      delay(MOTOR_WAIT);
      b = faderReadRAW(fader);
      //if( Serial ) Serial.print(fader); if( Serial ) Serial.print(" 2 "); if( Serial ) Serial.print(a); if( Serial ) Serial.print(","); if( Serial ) Serial.println(b);
    }
    fadersMax[fader] = faderReadRAW(fader);
    motorStop(fader);
    motorMove(fader, MOTOR_BACKWARD);
    a = 0; b = 10;
    while( a != b ) {
      a = faderReadRAW(fader);
      delay(MOTOR_WAIT);
      b = faderReadRAW(fader);
      //if( Serial ) Serial.print(fader); if( Serial ) Serial.print(" 3 "); if( Serial ) Serial.print(a); if( Serial ) Serial.print(","); if( Serial ) Serial.println(b);
    }
    fadersMin[fader] = faderReadRAW(fader);
    motorStop(fader);
    if( Serial ) Serial.print("- calibrate fader "); if( Serial ) Serial.print(fader); if( Serial ) Serial.print(": "); if( Serial ) Serial.print(fadersMin[fader]); if( Serial ) Serial.print(" to "); if( Serial ) Serial.println(fadersMax[fader]);
  }
#else
  for( byte fader = 0; fader < FADER_COUNT; fader++ ) {
    fadersMin[fader] = MOTOR_DEFAULTMIN;
    fadersMax[fader] = MOTOR_DEFAULTMAX;
  }
#endif
  if( Serial ) Serial.println(" Motors ready");
}

void setModeOfPins(byte pins[], int mode) {
  // TODO: Why do I need <= here and < in the ones which use a default array #confused
  for( int pin=0; pin <= sizeof(pins); pin++ ) {
    //if( Serial ) Serial.print("Pin Mode "); if( Serial ) Serial.print(pins[pin]); if( Serial ) Serial.print(" to "); if( Serial ) Serial.println(mode);
    pinMode(pins[pin], mode);
  }
}

void motorSelect(byte fader) {
  for( int bt=0; bt<sizeof(motorAddressPins); bt++ ) {
    byte state = bitRead(fader, bt);
    //if( Serial ) Serial.print("Pin "); if( Serial ) Serial.print(motorAddressPins[bt]); if( Serial ) Serial.print(" to "); if( Serial ) Serial.println(state);
    digitalWrite(motorAddressPins[bt], state);
  }
}

void motorMove(byte fader, byte dir) {
  motorSelect(fader);
  digitalWrite(MOTOR_CTLA,(dir == MOTOR_FORWARD ? HIGH : LOW));
  digitalWrite(MOTOR_CTLB,(dir == MOTOR_FORWARD ? LOW : HIGH));
}

void motorStop(byte fader) {
  motorSelect(fader);
  digitalWrite(MOTOR_CTLA,LOW);
  digitalWrite(MOTOR_CTLB,LOW);  
}

void faderSelect(byte fader) {
  for( int bt=0; bt<sizeof(faderAddressPins); bt++ ) {
    byte state = bitRead(fader, bt);
    digitalWrite(faderAddressPins[bt], state);
  }
}

double faderReadRAW(byte fader) {
  faderSelect(fader);
  return analogRead(FaderPins[0]);
}

byte faderRead(byte fader) {
  faderSelect(fader);
  double val = analogRead(FaderPins[0]);
  if( fadersMin[fader] < fadersMax[fader] ) {
    if( val < fadersMin[fader] ) {
      fadersMin[fader] = val;
    } else if( val > fadersMax[fader] ) { 
      fadersMax[fader] = val;
    }
  } else {
    if( val > fadersMin[fader] ) {
      fadersMin[fader] = val;
    } else if( val < fadersMax[fader] ) {
      fadersMax[fader] = val;
    }
  }
  return map(val, fadersMin[fader], fadersMax[fader], FADER_MIN, FADER_MAX);
}

