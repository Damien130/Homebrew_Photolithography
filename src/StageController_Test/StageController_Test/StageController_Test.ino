#include <Arduino.h>
#include <SPI.h>
#include <Streaming.h>
#include <TMC429.h>
#include <TMC2209.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

HardwareSerial * serial_stream_ptrs[2] = 
{
  &Serial1,
  &Serial2,
};
uint16_t stall_guard_resultX = 1000, stall_guard_resultY = 1000;

const int STALL_THRESHOLD_X = 200, STALL_THRESHOLD_Y = 170;
const int RUN_CURRENT_PERCENT = 100;

const int DELAY = 2000;
const static int8_t CHIP_SELECT_PIN = 53;
const static int8_t POS_COMP_PIN = 47;
const int MICROSTEPS_PER_STEP = 256;
const int COOL_STEP_DURATION_THRESHOLD = 360; // lower bound for stallguard check


const int CLOCK_FREQUENCY_MHZ = 16; // clock for step frequency
const int STEPS_PER_REV = 200; // 1.8 degrees per step
const int REVS_PER_SEC_MAX = 5; // max revolution per seconds
const int ACCELERATION_MAX = 1000; // 50 kHz, accelerate from 0 to max
const int MICROSTEPS_PER_REV = STEPS_PER_REV * MICROSTEPS_PER_STEP;
// const long VELOCITY_MAX = MICROSTEPS_PER_REV * REVS_PER_SEC_MAX;
const long VELOCITY_MAX = 50000;
const long VELOCITY_MIN = 360;
const long AXIS_LENGTH = 200000000; // 2e8 nm (200mm) per axis
const long LENGTH_PER_REV = 5000000; // 5e6 nm (5mm) per revolution
const long REV_PER_AXIS = AXIS_LENGTH / LENGTH_PER_REV; // get revolutions per axis
int32_t MICROSTEPS_PER_AXIS = REV_PER_AXIS * MICROSTEPS_PER_REV; 
int32_t MAX_CORR = 2045000; // 256 ustep
const int RUN_CURRENT = 100; // decrease if overheating
const int HOLD_CURRENT = 0;
const int HOLD_DELAY = 0;

const TMC2209::StandstillMode STANDSTILL_MODE = TMC2209::STRONG_BRAKING;
const int PWM_OFFSET = 36;
const int PWM_GRADIENT = 0;
const TMC2209::CurrentIncrement COOL_STEP_CURRENT_INCREMENT = TMC2209::CURRENT_INCREMENT_4;
const int COOL_STEP_LOWER_THRESHOLD = 1;
const int COOL_STEP_UPPER_THRESHOLD = 0;

TMC429 stepper_controller;    // TMC429 Stepper Controller
TMC2209 stepper_drivers[2]; // TMC2209 stepper driver
//TMC429::Status status;  // TMC429 status struct

uint32_t pos_X = 0, pos_Y = 0;
bool homing_completeX = false, homing_completeY = false;

enum Flags
{ MOTOR_COUNT = 2,
  X_MOTOR = 0,
  Y_MOTOR = 1
};

void homing();

void setup() {

  pinMode(POS_COMP_PIN, INPUT); // set comp to input

  Serial.begin(115200);
  Serial3.begin(57600);
  // put your setup code here, to run once:

  for (size_t motor_index=0; motor_index<MOTOR_COUNT; ++motor_index)
  {
    HardwareSerial & serial_stream = *(serial_stream_ptrs[motor_index]);
    TMC2209 & stepper_driver = stepper_drivers[motor_index];
    stepper_driver.setup(serial_stream, 500000);

    stepper_driver.enableAutomaticCurrentScaling();
    stepper_driver.enableAutomaticGradientAdaptation();
    delay(140);
    stepper_driver.setRunCurrent(RUN_CURRENT_PERCENT);
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
    //stepper_controller.setActualPosition(motor_index, 0);
  }

  stepper_drivers[X_MOTOR].moveAtVelocity(-VELOCITY_MAX);
  stepper_drivers[Y_MOTOR].moveAtVelocity(-VELOCITY_MAX);
  delay(200);
}

void loop() {
  if(!homing_completeX || !homing_completeY) {
    stall_guard_resultX = stepper_drivers[X_MOTOR].getStallGuardResult();
    stall_guard_resultY = stepper_drivers[Y_MOTOR].getStallGuardResult();
    if(stall_guard_resultX < STALL_THRESHOLD_X) {
      stepper_drivers[X_MOTOR].moveAtVelocity(0);
      homing_completeX = true;
    }
    if (stall_guard_resultY < STALL_THRESHOLD_Y) {
      stepper_drivers[Y_MOTOR].moveAtVelocity(0);
      homing_completeY = true;
    }
  } else {
/* if (stepper_controller.getActualPosition(X_MOTOR) == MAX_CORR) {
    stepper_controller.setTargetPosition(X_MOTOR, 0);
    stepper_controller.setTargetPosition(Y_MOTOR, 0);
  } else if (stepper_controller.getActualPosition(X_MOTOR) == 0){ 
    stepper_controller.setTargetPosition(X_MOTOR, MAX_CORR);
    stepper_controller.setTargetPosition(Y_MOTOR, MAX_CORR);
  }
  
  for (size_t motor_index = 0; motor_index < MOTOR_COUNT; ++motor_index) {
    pos_X = stepper_controller.getActualPosition(X_MOTOR);
    pos_Y = stepper_controller.getActualPosition(Y_MOTOR);
    Serial3.print("X_Act_Pos");
    Serial3.println(pos_X);
    Serial3.println("");
    Serial3.print("Y_Act_Pos");
    Serial3.println(pos_Y); */
    stepper_controller.setTargetPosition(X_MOTOR, MAX_CORR/2);
    stepper_controller.setTargetPosition(Y_MOTOR, MAX_CORR/2 + 307200);
  

  /* for (size_t motor_index=0; motor_index<MOTOR_COUNT; ++motor_index)
  {
    HardwareSerial & serial_stream = *(serial_stream_ptrs[motor_index]);
    TMC2209 & stepper_driver = stepper_drivers[motor_index];
    Serial.print("run_current_percent = ");
    Serial.println(RUN_CURRENT_PERCENT);

    Serial.print("stall_guard_threshold for motor #");
    Serial.print(motor_index);
    Serial.print(" ");
    Serial.println(STALL_THRESHOLD);

    uint16_t stall_guard_result = stepper_driver.getStallGuardResult();
    Serial.print("stall_guard_result = ");
    Serial.println(stall_guard_result);

    Serial.println("");
  } */

  delay(DELAY);
  }
  
 }


void homing() {

}

