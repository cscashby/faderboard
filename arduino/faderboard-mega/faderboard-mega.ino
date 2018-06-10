// General libraries
#include <SPI.h>

// Radio libraries
#include <RFM69.h>
//#include <RFM69_ATC.h>

// Libraries for displays
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1331.h>

#include "MemoryFree.h"

#define SERIAL_BAUD   115200

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
#define FADER_PIN     A0
// TODO: Remove these in favour of a global BEN
#define MOTOR_BEN     10
#define FADER_BEN     25

// Need to check what's best for OSC here
#define FADER_MIN     0
#define FADER_MAX     255

const byte displayAddressPins[] = {26,27,28};
const byte motorAddressPins[] = {7,8,9};
const byte faderAddressPins[] = {22,23,24};
const byte MApins[] = {7,8,9};

// Constants
#define MOTOR_WAIT  10

#define MOTOR_FORWARD 1
#define MOTOR_BACKWARD 0

// Uncomment this for faders to calibrate using motors
// - if commented, picks a guess (below) for max / min (quieter and quicker)
// - faders will calibrate first time they're used
//#define MOTOR_CALIBRATE
#define MOTOR_DEFAULTMIN  900
#define MOTOR_DEFAULTMAX  100

double fadersMin[FADER_COUNT];
double fadersMax[FADER_COUNT];

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
  // Initialise displays
  initDisplays();
}

// Main loop
unsigned long previousMillis = 0;
const long sendInterval = 3000;
void loop() {
  Adafruit_SSD1331* currentDisplay;
  for( byte fader = 0; fader < DISPLAY_COUNT; fader++ ) {
    currentDisplay = getDisplay(fader);
    currentDisplay->setTextSize(2);
    currentDisplay->setCursor(0,35);
    currentDisplay->setTextColor(RED, BLACK);
    currentDisplay->print(faderRead(fader));
  }

  // Send test radio packet     
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= sendInterval) {
    previousMillis = currentMillis;
  
    if (Serial) Serial.print("Sending");
    char payload[] = "hello from test node";
    if (radio.sendWithRetry(RADIO_DESKID, payload, sizeof(payload), 1, 200)) {
      if (Serial) Serial.println("... ACK received");
    } else {
      if (Serial) Serial.println("... No ACK");
    }
  }
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
  #ifdef IS_RFM69HW_HCW
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

void initFaders() {
  if( Serial ) Serial.println("Motor initialisation...");
  pinMode(MOTOR_CTLA,OUTPUT);
  pinMode(MOTOR_CTLB,OUTPUT);
  // - TODO: Remove BEN
  pinMode(MOTOR_BEN,OUTPUT);
  pinMode(FADER_BEN,OUTPUT);
  digitalWrite(MOTOR_BEN, HIGH);
  digitalWrite(FADER_BEN, LOW);
  setModeOfPins(motorAddressPins, OUTPUT);
  setModeOfPins(faderAddressPins, OUTPUT);
#ifdef MOTOR_CALIBRATE
  // - calibration
  //   - before / after fader variables to check if stopped moving
  double a,b;
  for( byte fader = 0; fader < FADER_COUNT; fader++ ) {
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
  return analogRead(FADER_PIN);
}

double faderRead(byte fader) {
  faderSelect(fader);
  double val = analogRead(FADER_PIN);
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

