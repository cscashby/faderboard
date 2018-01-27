#include <CapacitiveSensor.h>

int sensorPin = A0;    // select the input pin for the potentiometer
int ledPin = 13;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor

const int touchSend    = 7;   //Send pin for Capacitance Sensing Circuit (Digital 7)
const int touchReceive = 8;   //Receive pin for Capacitance Sensing Circuit (Digital 8)

CapacitiveSensor touchLine = CapacitiveSensor(touchSend, touchReceive);

void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // read the value from the sensor:
  sensorValue = analogRead(sensorPin);
  delay(5);
  Serial.print(sensorValue);
  int t = touchLine.capacitiveSensor(30);
  if( t > 700 ) {
    Serial.print("  touch  ");
  } else {
    Serial.print("  nope   ");
  }
  Serial.println(t);
  
  delay(5);
}
