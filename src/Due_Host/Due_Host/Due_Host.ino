#include <Wire.h>

uint64_t frame = 0x5A01AAAAAAAAFF00;
// Magic, Command, Data, Data, Data, Data, Status, CRC

uint8_t counter = 0;
uint8_t packet = 0;
char debugBuffer[50];

void setup() {
  Wire.begin();
  Serial.begin(15200); // start serial for console
}

void loop() {
  Wire.beginTransmission(0x0F);
  Wire.write(reinterpret_cast<uint8_t*>(&frame), 8);
  Wire.endTransmission();

  Wire.requestFrom(0x0F, 8); // request 8 bytes from 0x0F
  while (Wire.available()) {
    packet = Wire.read();
    sprintf(debugBuffer, "Packet %d -> %p", counter, packet);
    Serial.println(debugBuffer);
    counter++;
  }
  counter = 0;
  delay(5000);
}