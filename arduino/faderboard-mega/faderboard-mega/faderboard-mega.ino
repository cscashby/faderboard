// General libraries
#include <SPI.h>

// Libraries for displays
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1331.h>

// Pin assignments
// - TODO: Make these one, but I'm testing with random numbers of things populated
#define DISPLAY_COUNT  4
#define FADER_COUNT    8

#define DISPLAY_CS    53
#define DISPLAY_RST   48
#define DISPLAY_DC    49
#define MOTOR_CTLA     5
#define MOTOR_CTLB     6
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
  Serial.begin(9600);
  Serial.println("START");

  // Initialise fader motors
  initFaders();
  // Initialise displays
  initDisplays();
  // Initialise LoRa radio stuff
}

void loop() {
  Adafruit_SSD1331* currentDisplay;
  for( byte fader = 0; fader < DISPLAY_COUNT; fader++ ) {
    currentDisplay = getDisplay(fader);
    currentDisplay->setTextSize(2);
    currentDisplay->setCursor(0,35);
    currentDisplay->setTextColor(RED, BLACK);
    currentDisplay->print(faderRead(fader));
  }
}

void initDisplays() {
  Serial.println("-display initialisation");
  // - set DA pins as output
  setModeOfPins(displayAddressPins, OUTPUT);
  for( byte fader = 0; fader < DISPLAY_COUNT; fader++ ) {
    for( int bt=0; bt<=sizeof(displayAddressPins); bt++ ) {
      byte state = bitRead(fader, bt);
      digitalWrite(displayAddressPins[bt], state);
    }
    Adafruit_SSD1331 currentDisplay = Adafruit_SSD1331(DISPLAY_CS, DISPLAY_DC, DISPLAY_RST);
    displays[fader] = &currentDisplay;
    currentDisplay.begin();
    currentDisplay.fillScreen(BLACK);
    currentDisplay.setTextColor(BLUE);
    currentDisplay.setTextSize(3);
    currentDisplay.println(fader);
    currentDisplay.setTextSize(1);
    currentDisplay.print(fadersMin[fader]); currentDisplay.print(" - "); currentDisplay.print(fadersMax[fader]);
  }
}

Adafruit_SSD1331* getDisplay(byte fader) {
  for( int bt=0; bt<=sizeof(displayAddressPins); bt++ ) {
    byte state = bitRead(fader, bt);
    digitalWrite(displayAddressPins[bt], state);
  }
  return displays[fader];
}

void initFaders() {
  Serial.println("-motor initialisation");
  pinMode(MOTOR_CTLA,OUTPUT);
  pinMode(MOTOR_CTLB,OUTPUT);
  // - TODO: Remove BEN
  pinMode(MOTOR_BEN,OUTPUT);
  pinMode(FADER_BEN,OUTPUT);
  digitalWrite(MOTOR_BEN, HIGH);
  digitalWrite(FADER_BEN, LOW);
  setModeOfPins(motorAddressPins, OUTPUT);
  setModeOfPins(faderAddressPins, OUTPUT);
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
      //Serial.print(fader); Serial.print(" 1 "); Serial.print(a); Serial.print(","); Serial.println(b);
    }
    a = 0; b = 10;
    motorMove(fader, MOTOR_FORWARD);
    while( a != b ) {
      a = faderReadRAW(fader);
      delay(MOTOR_WAIT);
      b = faderReadRAW(fader);
      //Serial.print(fader); Serial.print(" 2 "); Serial.print(a); Serial.print(","); Serial.println(b);
    }
    fadersMax[fader] = faderReadRAW(fader);
    motorStop(fader);
    motorMove(fader, MOTOR_BACKWARD);
    a = 0; b = 10;
    while( a != b ) {
      a = faderReadRAW(fader);
      delay(MOTOR_WAIT);
      b = faderReadRAW(fader);
      //Serial.print(fader); Serial.print(" 3 "); Serial.print(a); Serial.print(","); Serial.println(b);
    }
    fadersMin[fader] = faderReadRAW(fader);
    motorStop(fader);
    Serial.print("Calibrate fader "); Serial.print(fader); Serial.print(": "); Serial.print(fadersMin[fader]); Serial.print(" to "); Serial.println(fadersMax[fader]);
  }
}

void setModeOfPins(byte pins[], int mode) {
  // TODO: Why do I need <= here and < in the ones which use a default array #confused
  for( int pin=0; pin <= sizeof(pins); pin++ ) {
    //Serial.print("Pin Mode "); Serial.print(pins[pin]); Serial.print(" to "); Serial.println(mode);
    pinMode(pins[pin], mode);
  }
}

void motorSelect(byte fader) {
  for( int bt=0; bt<sizeof(motorAddressPins); bt++ ) {
    byte state = bitRead(fader, bt);
    //Serial.print("Pin "); Serial.print(motorAddressPins[bt]); Serial.print(" to "); Serial.println(state);
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

