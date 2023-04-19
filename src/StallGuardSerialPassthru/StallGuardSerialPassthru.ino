#include <SoftwareSerial.h>
#include <Arduino.h>
#include <TMC2209.h>

#define RX 37
#define TX 36

SoftwareSerial console(RX,TX);
HardwareSerial & serial_stream = Serial2;

const int DELAY = 200;
// current values may need to be reduced to prevent overheating depending on
// specific motor and power supply voltage
const uint8_t RUN_CURRENT_PERCENT = 100;
const int32_t VELOCITY = 20000;
const uint8_t STALL_GUARD_THRESHOLD = 20;

TMC2209 stepper_driver;

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600);

  pinMode(RX, INPUT);
  pinMode(TX, OUTPUT);
  console.begin(115200);

  stepper_driver.setup(serial_stream);
  stepper_driver.setRunCurrent(RUN_CURRENT_PERCENT);
  stepper_driver.setStallGuardThreshold(STALL_GUARD_THRESHOLD);
  stepper_driver.enable();
  stepper_driver.moveAtVelocity(VELOCITY);
}

void loop() {
/*   if (Serial.available()) {        // If anything comes in Serial (USB),
    Serial1.write(Serial.read());  // read it and send it out Serial1 (pins 0 & 1)
  }

  if (Serial1.available()) {       // If anything comes in Serial1 (pins 0 & 1)
    Serial.write(Serial1.read());  // read it and send it out Serial (USB)
  }
 */
  Serial.print("run_current_percent = ");
  Serial.println(RUN_CURRENT_PERCENT);

  Serial.print("stall_guard_threshold = ");
  Serial.println(STALL_GUARD_THRESHOLD);

  uint16_t stall_guard_result = stepper_driver.getStallGuardResult();
  Serial.print("stall_guard_result = ");
  Serial.println(stall_guard_result);

  Serial.println("");
  delay(DELAY);
}
