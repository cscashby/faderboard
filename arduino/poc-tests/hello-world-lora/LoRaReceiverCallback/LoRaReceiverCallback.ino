#include <SPI.h>
#include <LoRa.h>

//LoRa32u4 ports
#define SS      8
#define RST     4
#define DI0     7
#define BAND    433E6 
#define SPIFREQ 8E6

void setup() {
  Serial.begin(9600);
  while (!Serial);

  LoRa.setSPIFrequency(SPIFREQ);
  LoRa.setPins(SS,RST,DI0);
  
  Serial.print("LoRa Receiver ");
  Serial.println(LoRa.random());

  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  
  // register the receive callback
  LoRa.onReceive(onReceive);

  // put the radio into receive mode
  LoRa.receive();
}

void loop() {
  // do nothing
}

unsigned long last = 0UL;
int on = true;

void onReceive(int packetSize) {
  // received a packet
  Serial.print("Received packet '");

  if( on ) {
    digitalWrite(LED_BUILTIN, HIGH);
    on = false;
  } else {
    digitalWrite(LED_BUILTIN, LOW);
    on = true;
  }

  String s = "";

  // read packet
  for (int i = 0; i < packetSize; i++) {
    s.concat((char)LoRa.read());
  }
  // print RSSI of packet
  Serial.print(s);
  Serial.print(" at ");
  Serial.print(String(millis() - last));
  Serial.print("' with RSSI ");
  Serial.println(LoRa.packetRssi());
  
  last = millis();
}
