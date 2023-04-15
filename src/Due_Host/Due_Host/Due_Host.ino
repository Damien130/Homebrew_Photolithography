#include <Wire.h>

uint64_t frame = 0x5A01AAAAAAAAFF00;
uint64_t inv_frame = 0x00FFFFFFFFFF055A;

uint64_t testList[9] = 
{
  0x00FFFFFFFFFF005A, // halt
  0x00FFFFFFFFFF015A, // getWidth
  0x00FFFFFFFFFF025A, // getHeight
  0x00FFFFFFFFFF035A, // getX
  0x00FFFFFFFFFF045A, // getY
  0x00FF04277DC9055A, // setX
  0x00FFFFFFFFFF065A, // setY
  0x00FFFFFFFFFF075A, // Calib
  0x000000000000075A, // CalibComp
};
// Magic, Command, Data, Data, Data, Data, Status, CRC

uint8_t counter = 0, packet = 0, commandInd = 0;
char debugBuffer[50];

void setup() {
  // Wire.setClock(100000);
  Wire.begin();
  Serial.begin(9600); // start serial for console
}

void loop() {
  if(commandInd == 9) commandInd = 0;
  Wire.beginTransmission(0x0F);
  Wire.write(reinterpret_cast<uint8_t*>(&testList[commandInd]), 8);
  Wire.endTransmission();
  commandInd++;

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