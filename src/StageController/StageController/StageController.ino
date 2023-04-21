/*************************************************************
 * Photolithography Stepper Stage Controller
 * Author: Damien Hu, damien@damienhu.com
 * 
 * This program is free software: you can redistribute it 
 * and/or modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation, either version 3 of 
 * the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 * 
**************************************************************/

#include <Arduino.h>
#include <digitalWriteFast.h>
#include <TMC429.h>
#include <TMC2209.h>
#include <Wire.h> // EDIT TWI.C TO REMOVE INTERNAL PULL UP!!!!!!!
#include <stdint.h>
#include <stdio.h>
#include <SPI.h>
#include <Streaming.h>


/* ATMega2560 Pin Mapping */
/* Digital Pins:
  RESET: RESET pin for emergency stop
  2: TMC2209_X error flag (PE4, 6) Interrupt 5 
  3: TMC2209_Y error flag (PE5, 7) Interrupt 6
  49: TMC429 Chip Select (PL0, 35)
*/

const static int8_t CHIP_SELECT_PIN = 53;
const static int8_t I2CADDR = 0x0F; 
const long SERIAL_BAUD_RATE = 115200;

/* Data structure
Byte1: scribble: magic number
Byte2: Command
Byte3-6: data (4 bytes)
Byte7: 0x00
Byte8: Chksum 
 */

void print_uint64_t(uint64_t num);
enum Flags
{ MOTOR_COUNT = 2,
  X_MOTOR = 0,
  Y_MOTOR = 1
};
bool homing = false;
int32_t X_target = 0, X_actual = 0, Y_target = 0, Y_actual = 0, X_command = 0, Y_command = 0; // plane location, initialize to 0
int32_t X_latched = 0, Y_latched = 0;
bool stageHalt = false;
bool calibration = false;
bool calibrationComplete = false;
int8_t query = 0;
uint16_t stall_guard_resultX = 1000, stall_guard_resultY = 1000;
const int STALL_THRESHOLD_X = 170, STALL_THRESHOLD_Y = 170;

/* TMC2209 driver settings ==================================================================*/
/* Note: Current TMC2209 setup doesn't utilize UART address for simplicity of troubleshooting
   Instead, UART channel 1 and 2 is used for separate controllers 
   NOT gate required for output of TMC2209 error detection */
HardwareSerial * serial_stream_ptrs[MOTOR_COUNT] =
{
  &Serial1,
  &Serial2,
};
const int CLOCK_FREQUENCY_MHZ = 16; // clock for step frequency
const int DELAY = 2000;
const int RUN_CURRENT = 100; // decrease if overheating
const int HOLD_CURRENT = 0;
const int HOLD_DELAY = 0;
const int MICROSTEPS_PER_STEP = 256; // 256 microstep per full step
const TMC2209::StandstillMode STANDSTILL_MODE = TMC2209::STRONG_BRAKING;
const int PWM_OFFSET = 36;
const int PWM_GRADIENT = 0;
const int COOL_STEP_DURATION_THRESHOLD = 2000;
const TMC2209::CurrentIncrement COOL_STEP_CURRENT_INCREMENT = TMC2209::CURRENT_INCREMENT_4;
const int COOL_STEP_LOWER_THRESHOLD = 1;
const int COOL_STEP_UPPER_THRESHOLD = 0;

/* TMC429 controller settings
   TMC429 Velocity Calculation: 
   Maximum Microstep Rate: 500 kHz (16 MHz clock / 32)
*/
const int STEPS_PER_REV = 200; // 1.8 degrees per step
const int REVS_PER_SEC_MAX = 1; // max revolution per seconds
const int ACCELERATION_MAX = 1000; 
const int MICROSTEPS_PER_REV = STEPS_PER_REV * MICROSTEPS_PER_STEP;
const long VELOCITY_MAX = 50000;
const long VELOCITY_MIN = 360;
const long AXIS_LENGTH = 200000000; // 2e8 nm (200mm) per axis
const long LENGTH_PER_REV = 5000000; // 5e6 nm (5mm) per revolution
const long REV_PER_AXIS = AXIS_LENGTH / LENGTH_PER_REV; // get revolutions per axis
int32_t MICROSTEPS_PER_AXIS = 2045000; // get maximum microsteps per axis
bool homing_completeX = false, homing_completeY = false;

// const long DISTANCE_PER_MICROSTEP = LENGTH_PER_REV / MICROSTEPS_PER_REV; 
// 0.097 micron per step, float (4 bytes)

/* Instantiate stepper drive train*/
TMC2209 stepper_drivers[MOTOR_COUNT]; // TMC2209, setup as object array
TMC429 stepper_controller;    // TMC429 Stepper Controller
TMC429::Status status;  // TMC429 status struct



// INSTRUCTION HANDLER FUNCTIONS AND INITIALIZATION


#define SCRIBBLE 0x5a
uint64_t commandBuffer; // outgoing packet buffer
char debugBuffer[50]; // serial console buffer
bool debugMode = true;

void write();
void read();
void execute();
void dispBuffer();
void halt();
void getWidth();
void getHeight();
void getX();
void getY();
void setX();
void setY();
void calib();
uint8_t getCheckSum();
bool checkCheckSum();


void(*const functionMap[9])(void) = {
  &halt
  , &getWidth
  , &getHeight
  , &getX
  , &getY
  , &setX
  , &setY
  , &calib
};

void setup()
{ 
  //pinMode(POS_COMP_PIN, INPUT); // set comp to input
  Serial.begin(SERIAL_BAUD_RATE); // Serial console on serial0 @ 15200 baud
  Serial3.begin(57600); // Mac Console RS422
  Serial3.write(27);       // ESC command
  Serial3.print("[2J");    // clear screen command
  Serial3.write(27);
  Serial3.print("[H"); 
  Wire.begin(I2CADDR); // setup as slave with address I2CADDR
  Wire.onRequest(requestEvent); // request event
  Wire.onReceive(receiveEvent); // receive event

  pinMode (43, OUTPUT);
  digitalWriteFast(43, HIGH);




  /* Self Check */
  Serial3.write(27);       // ESC command
  Serial3.print("[2J");    // clear screen command
  Serial3.write(27);
  Serial3.print("[H"); 
  for (size_t motor_index = 0; motor_index < MOTOR_COUNT; ++motor_index)
  {
    HardwareSerial & serial_stream = *(serial_stream_ptrs[motor_index]); // set serial stream
    TMC2209 & stepper_driver = stepper_drivers[motor_index]; // setup alias for  object in object array
    stepper_driver.setup(serial_stream, 500000); // get communications going
    if (stepper_driver.isSetupAndCommunicating()) // if communicating
    {
      Serial3.print("TMC2226 # ");
      Serial3.print(motor_index, DEC);
      Serial3.print(" is up and running");
      Serial3.println("");
    } else {
      Serial3.print("TMC2226 # ");
      Serial3.print(motor_index, DEC);
      Serial3.print(" is not communicating \n");
    }
    stepper_driver.enableAutomaticCurrentScaling();
    stepper_driver.enableAutomaticGradientAdaptation();
    delay(200);
    stepper_driver.setRunCurrent(RUN_CURRENT);
    stepper_driver.setHoldCurrent(HOLD_CURRENT); // hold current setup

    stepper_driver.setStallGuardThreshold(STALL_THRESHOLD_X);
    stepper_driver.setMicrostepsPerStep(MICROSTEPS_PER_STEP);
    stepper_driver.enableStealthChop();

    stepper_driver.setCoolStepDurationThreshold(COOL_STEP_DURATION_THRESHOLD);
    stepper_driver.setCoolStepCurrentIncrement(COOL_STEP_CURRENT_INCREMENT);
    stepper_driver.enableCoolStep(COOL_STEP_LOWER_THRESHOLD,COOL_STEP_UPPER_THRESHOLD);
    //stepper_driver.setPwmOffset(PWM_OFFSET); 
    //stepper_driver.setPwmGradient(PWM_GRADIENT);
    stepper_driver.setStandstillMode(STANDSTILL_MODE);

    stepper_driver.setHoldDelay(HOLD_DELAY);
    stepper_driver.useExternalSenseResistors();
    stepper_driver.enable();
  }

  stepper_controller.setup(CHIP_SELECT_PIN,CLOCK_FREQUENCY_MHZ); // setup TMC429
  stepper_controller.initialize();  // initialize TMC429
  if (stepper_controller.communicating()) // if communicating
  {
    Serial3.println("TMC429 is up and communicating");
  } else {
    Serial3.println("TMC429 is not communicating");
  }

  /* TMC429 Setup */
  /* Left reference stop for motor 1 (X)
     Right reference stop for motor 2 (Y) */
  stepper_controller.setup(CHIP_SELECT_PIN,CLOCK_FREQUENCY_MHZ); 
  stepper_controller.initialize();  

  //stepper_controller.disableInverseStepPolarity(); // disables active low for STEP/DIR
  //stepper_controller.disableInverseDirPolarity();
  stepper_controller.setSwitchesActiveLow(); // the stop switches are active low - pass thru NOT gate
  //stepper_controller.setReferenceSwitchToLeft(X_MOTOR);
  //stepper_controller.setReferenceSwitchToRight(Y_MOTOR);
  stepper_controller.disableRightSwitchStop(X_MOTOR); // Right/left switch doesn't exist
  stepper_controller.disableLeftSwitchStop(X_MOTOR);
  stepper_controller.disableRightSwitchStop(Y_MOTOR);
  stepper_controller.disableLeftSwitchStop(Y_MOTOR);
  stepper_controller.setPositionCompareMotor(X_MOTOR);

  for (size_t motor_index=0; motor_index < MOTOR_COUNT; ++motor_index)
  {
    stepper_controller.disableSwitchSoftStop(motor_index); // stop immediately upon stall
    stepper_controller.setRampMode(motor_index); // ramp mode for internal uStep plane
    stepper_controller.setLimitsInHz(motor_index,VELOCITY_MIN,VELOCITY_MAX,ACCELERATION_MAX);
  }
  
  
  stepper_drivers[X_MOTOR].moveAtVelocity(-VELOCITY_MAX);
  stepper_drivers[Y_MOTOR].moveAtVelocity(-VELOCITY_MAX);
  delay(200);
  
  Serial3.println("Homing procedure in progress, please wait..."); 

}

void loop()
{
  /* if(halt) {
    stepper_controller.stopAll();
  } */
  
  /* // move the motors
  if (!calibration) { 
    // check for command difference, if different, move to location defined
    if (X_target != X_command) {
      stepper_controller.setTargetPosition(X_MOTOR, X_command);
    } else if (Y_target != Y_command) {
      stepper_controller.setTargetPosition(Y_MOTOR, Y_command);
    }
  } else {
    X_latched = stepper_controller.getActualPosition(X_MOTOR);
    Y_latched = stepper_controller.getActualPosition(Y_MOTOR);
    // move motors as requested
    if (X_target != X_command) {
      stepper_controller.setTargetPosition(X_MOTOR, X_command);
    } else if (Y_target != Y_command) {
      stepper_controller.setTargetPosition(Y_MOTOR, Y_command);
    }
    if (calibrationComplete) {
      stepper_controller.setActualPosition(X_MOTOR, X_latched);
      stepper_controller.setActualPosition(Y_MOTOR, Y_latched);
    }
  } */
  if(!homing_completeX || !homing_completeY) {
    stall_guard_resultX = stepper_drivers[X_MOTOR].getStallGuardResult();
    stall_guard_resultY = stepper_drivers[Y_MOTOR].getStallGuardResult();
    if(stall_guard_resultX < STALL_THRESHOLD_X) {
      stepper_drivers[X_MOTOR].moveAtVelocity(0);
      homing_completeX = true;
      stepper_controller.setActualPosition(X_MOTOR, 0);
    }
    if (stall_guard_resultY < STALL_THRESHOLD_Y) {
      stepper_drivers[Y_MOTOR].moveAtVelocity(0);
      homing_completeY = true;
      stepper_controller.setActualPosition(Y_MOTOR, 0);
    }
  } else if (homing_completeX && homing_completeY && !homing) {
    homing = true;
    stepper_controller.setTargetPosition(X_MOTOR, MICROSTEPS_PER_AXIS/2);
    stepper_controller.setTargetPosition(Y_MOTOR, MICROSTEPS_PER_AXIS/2);
  } else {
    X_target = stepper_controller.getTargetPosition(X_MOTOR);
    Y_target = stepper_controller.getTargetPosition(Y_MOTOR);

    // update controller status
    status = stepper_controller.getStatus();
    if (status.at_target_position_0 && status.at_target_position_1) {
      query = 2; // 0b10, stage finished moving
    } else {
      query = 1; // 0b01, stage moving
    }
    X_actual = stepper_controller.getActualPosition(X_MOTOR);
    Y_actual = stepper_controller.getActualPosition(Y_MOTOR);
    if (X_actual == MICROSTEPS_PER_AXIS/2 && 
        Y_actual == MICROSTEPS_PER_AXIS/2 && query == 0) {
    Serial3.println("Homing Complete!");
    query = 2;
    }
    if (query = 2) {
      Serial3.print("X Axis: ");
      Serial3.println(X_actual);
      Serial3.print("Y Axis: ");
      Serial3.println(Y_actual);
      Serial3.println("");
    }
    delay(DELAY);
  }

}

void moveStageTo(int32_t x, int32_t y)
{
  stepper_controller.setTargetPosition(X_MOTOR, x); // set X target coordinate
  stepper_controller.setTargetPosition(Y_MOTOR, y); // set Y target coordinate
}

void receiveEvent(int howMany)
{
  read();
  execute();
}

void requestEvent()
{
  write();
}

// I2C functions
void read() {
  uint8_t currRead = 0;
  //check 1st byte and compare to scribble
  for(size_t i = 0; i <= 50000; ++i) {
    if(i == 50000) {
      sprintf(debugBuffer, "read()-> timeout, read failed"); // try 5000 times to read buffer
      dispBuffer();
      commandBuffer = 0; // if nothing was received break
      return;
    }
    if (Wire.available() == 0) continue;
    currRead = static_cast<uint8_t>(Wire.read()); // wait for scribble to appear
    sprintf(debugBuffer, "read()-> first byte = %02X", currRead);
    dispBuffer();
    if(currRead == SCRIBBLE) {
      commandBuffer = static_cast<uint64_t>(SCRIBBLE); // reset command buffer
      break;
    }
  }

  // Wait for the buffer to receive at least 7 bytes
  while(Wire.available() < 7) {};
  // read bytes 2-8
  for(size_t i = 1; i < 8; i++) { // wire.read returns one byte at a time from 32-byte hardware buffer
    currRead = static_cast<uint8_t>(Wire.read()); // return one byte
    commandBuffer &= ~(static_cast<uint64_t>(0b11111111) << (8*i)); // Clear designated byte
    commandBuffer |= static_cast<uint64_t>(currRead) << (8*i); // Set command buffer
    sprintf(debugBuffer, "read()-> byte #%d = %02X", i, currRead);
    dispBuffer();
  }
  sprintf(debugBuffer, "read()-> read success");
  dispBuffer();
  // sprintf(debugBuffer, "read()-> 8-byte value: %08X%08X", commandBuffer >> 32, commandBuffer & 0xFFFFFFFF);
}

void write() {
  if(commandBuffer == 0) {
    sprintf(debugBuffer, "write()-> command buffer is empty, write failed");
    dispBuffer();
  }
  commandBuffer &= ~(static_cast<uint64_t>(0b11111111) << (8 * 6)); // clear status byte
  commandBuffer |= static_cast<uint64_t>(query) << (8 * 6); // update status
  commandBuffer &= ~(static_cast<uint64_t>(0b11111111) << (8 * 7)); // clear Chksum
  commandBuffer |= static_cast<uint64_t>(getCheckSum()) << (8 * 7); // update checksum

  /* uint8_t tmp;
  uint8_t *buf = (uint8_t *)&commandBuffer;
  for(int i = 0; i < 4; i++) {
    tmp = buf[i];
    buf[i] = buf[7-i];
    buf[7-i] = tmp;
  } */

  // sprintf(debugBuffer, "write() -> 8-byte value: %08X%08X", commandBuffer >> 32, commandBuffer & 0xFFFFFFFF);
  int ret = Wire.write(reinterpret_cast<uint8_t*>(&commandBuffer), 8);
  sprintf(debugBuffer, "write() -> bytes written: %d", ret);
  dispBuffer();
}

void execute() {
  size_t funcInd = *(reinterpret_cast<uint8_t*>(&commandBuffer) + 1);
  Serial.println("the func ind is");
  Serial.println(funcInd);
  if(funcInd <0 || funcInd >= 9) {
    sprintf(debugBuffer, "execute()-> invalid command: %d", static_cast<int>(funcInd));
    dispBuffer();
  }
  void (*func)(void) = *(functionMap + funcInd); // pull reference from le function map
  (*func)(); // execute the commanded function
}

void dispBuffer() { 
  if(debugMode) Serial.println(debugBuffer); // send debug buffer to Serial0
}

void halt() { // fixed
  stageHalt = true;
  sprintf(debugBuffer, "halt()-> halt status: %d", stageHalt); 
  dispBuffer();
}

void getWidth() { // fixed
  commandBuffer &= ~(static_cast<uint64_t>(0xFFFFFFFF) << (8 * 2)); // Clear data
  commandBuffer |= static_cast<uint64_t>(MICROSTEPS_PER_AXIS) << (8 * 2); // Set command buffer
  sprintf(debugBuffer, "getWidth()-> Width is %lu", MICROSTEPS_PER_AXIS); // MSB gets sent last
  dispBuffer();
}

void getHeight() { // fixed
  commandBuffer &= ~(static_cast<uint64_t>(0xFFFFFFFF) << (8 * 2)); // Clear data
  commandBuffer |= static_cast<uint64_t>(MICROSTEPS_PER_AXIS) << (8 * 2); // Set command buffer
  sprintf(debugBuffer, "getHeight()-> Height is %lu", MICROSTEPS_PER_AXIS);
  dispBuffer();
}


void getX() { // fixed
  commandBuffer &= ~(static_cast<uint64_t>(0xFFFFFFFF) << (8 * 2)); // Clear data
  commandBuffer |= static_cast<uint64_t>(X_actual) << (8 * 2); // Set command buffer
  sprintf(debugBuffer, "getX()-> CurrentX is %lu", X_actual);
  dispBuffer();
}

void getY() { // fixed
  commandBuffer &= ~(static_cast<uint64_t>(0xFFFFFFFF) << (8 * 2)); // Clear data
  commandBuffer |= static_cast<uint64_t>(Y_actual) << (8 * 2); // Set command buffer
  sprintf(debugBuffer, "getY()-> CurrentY is %lu", Y_actual);
  dispBuffer();
}

void setX() { // fixed
  X_command = *reinterpret_cast<int32_t*>((reinterpret_cast<uint8_t*>(&commandBuffer) + 2));
  stepper_controller.setTargetPosition(X_MOTOR, X_command); // set X target coordinate
  sprintf(debugBuffer, "SetX()-> X command set to: %ld", X_command);
  dispBuffer();
}

void setY() { // fixed
  Y_command = *reinterpret_cast<int32_t*>((reinterpret_cast<uint8_t*>(&commandBuffer) + 2));
  stepper_controller.setTargetPosition(Y_MOTOR, Y_command); // set X target coordinate
  sprintf(debugBuffer, "SetY()-> Y command set to: %ld", Y_command);
  dispBuffer();
}

void calib() {
  print_uint64_t(commandBuffer);
  if (*reinterpret_cast<int32_t*>((reinterpret_cast<uint8_t*>(&commandBuffer) + 2)) > 0) {
    Serial.println("We are getting ones");
  } else {
    Serial.println("we got some zeroes");
  }
  // calibration = *reinterpret_cast<int32_t*>((reinterpret_cast<uint8_t*>(&commandBuffer) + 2)) > 0;
  // calibrationComplete = *reinterpret_cast<int32_t*>((reinterpret_cast<uint8_t*>(&commandBuffer) + 2)) == 0;
  sprintf(debugBuffer, "calib()-> Calibration: %d, CalibComplete: %d", calibration, calibrationComplete);
  dispBuffer();
}

uint8_t getCheckSum() {
  uint8_t chksum = 0xFF, sub = 0;
  for(size_t i= 0; i < 7; i++)
    sub += *(reinterpret_cast<uint8_t*>(&commandBuffer) + i);
  chksum = chksum - sub;
  sprintf(debugBuffer, "getCheckSum()-> check-sum: %d", chksum);
  dispBuffer();
  return chksum;
}

uint32_t swapEndian(uint32_t unswapped){ // swap them if needed
  uint32_t swapped = ((unswapped>>24)&0xff) | // move byte 3 to byte 0
                    ((unswapped<<8)&0xff0000) | // move byte 1 to byte 2
                    ((unswapped>>8)&0xff00) | // move byte 2 to byte 1
                    ((unswapped<<24)&0xff000000); // byte 0 to byte 3
  
  return swapped;                    
}


void print_uint64_t(uint64_t num) {

   char rev[128]; 
   char *p = rev+1;

   while (num > 0) {
     *p++ = '0' + ( num % 10);
     num/= 10;
   }
   p--;
   /*Print the number which is now in reverse*/
   while (p > rev) {
     Serial.print(*p--);
   }
 }