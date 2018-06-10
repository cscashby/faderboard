#include <cscashby_CAP1296.h>

/**
  TCA9548 I2CScanner.pde -- I2C bus scanner for Arduino

  Based on code c. 2009, Tod E. Kurt, http://todbot.com/blog/

*/
#include "Wire.h"
extern "C" {
  #include "utility/twi.h" // from Wire library, so we can do bus scanning
}

#define TCAADDR 0x70

void tcaselect(uint8_t i) {
  if (i > 7) return;
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}

cscashby_CAP1296 cap = cscashby_CAP1296();

// standard Arduino setup()
void setup()
{
  while (!Serial);
  delay(1000);
  Wire.begin();
  Serial.begin(9600);
  Serial.println("\nTCAScanner ready!");
  for (uint8_t t = 0; t < 8; t++) {
    tcaselect(t);
    Serial.print("TCA Port #"); Serial.println(t);
    for (uint8_t addr = 0; addr <= 127; addr++) {
      if (addr == TCAADDR) continue;
      uint8_t data;
      if (! twi_writeTo(addr, &data, 0, 1, 1)) {
        Serial.print("Found I2C 0x"); Serial.println(addr, HEX);
      }
    }
  }
  Serial.println("\ndone");

  Serial.println("Testing #7");
  tcaselect(7);
  // Defaults to i2c address 0x28
  if (!cap.begin()) {
    Serial.println("CAP1296 not found");
    while (1);
  }
  Serial.println("CAP1296 found!");
}

void loop()
{
  uint8_t touched = cap.touched();

  if (touched == 0) {
    // No touch detected
    return;
  }
  
  for (uint8_t i=0; i<6; i++) {
    if (touched & (1 << i)) {
      Serial.print("C"); Serial.print(i+1); Serial.print("\t");
    }
  }
  Serial.println();
  delay(200);
}
