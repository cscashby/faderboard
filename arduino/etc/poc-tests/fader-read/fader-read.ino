const int sensorPin = A0;    // select the input pin for the potentiometer
const int ledPin = 13;      // select the pin for the LED

int sensorValue = 0;  // variable to store the value coming from the sensor

const int FADERS = 8;
const byte FApins[] = {22,23,24};
const byte FEN = 25;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  
  pinMode(FEN,OUTPUT);
  digitalWrite(FEN, LOW);
  for( int bt=0; bt<sizeof(FApins); bt++ ) {
    pinMode(FApins[bt], OUTPUT);
    Serial.print("pin ");
    Serial.print(FApins[bt]);
    Serial.print("=OUTPUT,");
  }
  Serial.println();
  Serial.println("START");
}

void loop() {
  Serial.println();
  for( int fader=0; fader<FADERS; fader++ ) { 
    Serial.print("Fader ");
    Serial.print(fader);
    Serial.print(" ");
    for( int bt=0; bt<sizeof(FApins); bt++ ) {
      byte state = bitRead(fader, bt);
      digitalWrite(FApins[bt], state);
      Serial.print(state);
    }
  
    sensorValue = analogRead(sensorPin);
    delay(5);
    Serial.print("=");
    Serial.print(sensorValue);
    Serial.print(",");
  }
}
