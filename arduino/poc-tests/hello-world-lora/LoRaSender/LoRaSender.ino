#include <SPI.h>
#include <LoRa.h>

//LoRa32u4 ports
#define SS      8
#define RST     4
#define DI0     7
#define BAND    433E6 
#define SPIFREQ 8E6

int counter = 0;

void setup() {
  //Serial.begin(9600);
  //while (!Serial);

  LoRa.setSPIFrequency(SPIFREQ);
  LoRa.setPins(SS,RST,DI0);
  
  //Serial.print("LoRa Sender ");
  //Serial.println(LoRa.random());

  if (!LoRa.begin(BAND)) {
  //  Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.print("Sending packet: ");
}

unsigned long last = 0UL;

void loop() {
  //Serial.print("Sending packet: ");
  //Serial.print(counter);

  // send packet
  LoRa.beginPacket();
  LoRa.print(last);
  //Serial.print(" at ");
  //Serial.println(String(millis() - last));
  LoRa.print(" ");
  LoRa.print(millis());
  LoRa.endPacket();

  counter++;

  delay(2);

  last = millis();
}

