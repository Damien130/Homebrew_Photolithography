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
#include "NeuBus.h"


/* ATMega2560 Pin Mapping */
/* Digital Pins:
  RESET: RESET pin for emergency stop
  2: TMC2209_X error flag (PE4, 6) Interrupt 5 
  3: TMC2209_Y error flag (PE5, 7) Interrupt 6
  49: TMC429 Chip Select (PL0, 35)

  ****** Introducing the NeuBus ******
  a simple backplane bus for the - Insert name here - 
  Bus host/controller: Raspberry Pi 4
  Bus attached device: ATMega2560 based stage controller
  Future expandability avaialble with buffers
  Maximum data width: 16 bits
  Maximum address width: 8 bits (Max 255 Commands)

  PORT D: Control Bus
  PD0: R/W' Read/Write, IRQ 0 INPUT
  PD1: IOSTRB' I/O Strobe, IRQ 1 INPUT
  PD2: INH' System inhibit, NMI-IRQ 2 INPUT
  PD3: ALE' Address Latch Enable, enable 16bit data bus, IRQ 3 INPUT
  PD4: IOCHRDY data bus ready, OUTPUT
  PD5: RDY device ready, OUTPUT
  PD6: INPOS stage in position, OUTPUT
  PD7: EXPRDY ready to expose, OUTPUT

  PORT A: Address Bus
  8 bits (PA0-PA7) used for addressing, INPUT
  used for upper 8 bits in 16-bit data bus mode

  PORT C: Data Bus
  8 bits (PC0-PC7) used for data, I/O depending on R/W' status
*/
enum pinAssignment
{
  CHIP_SELECT_PIN = 49,
  X_ERROR = 2,
  Y_ERROR = 3
};

/* Global variables */
uint8_t test = 0;
enum Flags
{ MOTOR_COUNT = 2,
  X_MOTOR = 1,
  Y_MOTOR = 2
};
const long SERIAL_BAUD_RATE = 115200;
volatile bool X_triggered = false, Y_triggered = false; // flag for interrupt routine
volatile bool RW = false, IOSTRB = false, INH = false, ALE = false; // Neubus ISR flags
volatile uint8_t accumulator = 0; // how many IRQs (TMC2209 stall) generated
int32_t X_target = 0, X_actual = 0, Y_target = 0, Y_actual = 0; // plane location, initialize to 0
int32_t X_max, Y_max; // maximum coordinates for each axis

/* TMC2209 driver settings */
/* Note: Current TMC2209 setup doesn't utilize UART address for simplicity of troubleshooting
   Instead, UART channel 1 and 2 is used for separate controllers 
   NOT gate required for output of TMC2209 error detection */
HardwareSerial * serial_stream_ptrs[MOTOR_COUNT] =
{
  &Serial2,
  &Serial3,
};
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
const int CLOCK_FREQUENCY_MHZ = 16; // clock for step frequency
const int STEPS_PER_QUARTER_REV = 50; // 1.8 degrees per step
const int QUARTER_REVS_PER_SEC_MAX = 1; // max revolution per seconds
const int INC_PER_REV = 5;
const int ACCELERATION_MAX = 50000/2; // 50 kHz, accelerate from 0 to max
const int MICROSTEPS_PER_QUARTER_REV = STEPS_PER_QUARTER_REV*MICROSTEPS_PER_STEP;
const int MICROSTEPS_PER_REV = MICROSTEPS_PER_QUARTER_REV * 4;
const long VELOCITY_MAX = MICROSTEPS_PER_QUARTER_REV*QUARTER_REVS_PER_SEC_MAX;
const long VELOCITY_MIN = 100;

/* Instantiate stepper drive train*/
TMC2209 stepper_drivers[MOTOR_COUNT]; // TMC2209, setup as object array
TMC429 stepper_controller;    // TMC429 Stepper Controller
 
/* Instantiate NeuBus interface*/
NeuBus Bus;

void setup()
{ 
  Serial.begin(SERIAL_BAUD_RATE); // Serial console on serial0 @ 15200 baud

  /* ATMega2560 Setup */
  pinMode(X_ERROR, INPUT);
  pinMode(Y_ERROR, INPUT);

  /* Self Check */
  for (size_t motor_index = 0; motor_index < MOTOR_COUNT; ++motor_index)
  {
    HardwareSerial & serial_stream = *(serial_stream_ptrs[motor_index]); // set serial stream
    TMC2209 & stepper_driver = stepper_drivers[motor_index]; // setup alias for  object in object array
    stepper_driver.setup(serial_stream); // get communications going
    if (stepper_driver.isSetupAndCommunicating()) // if communicating
    {
      Serial.print("TMC2209 # ");
      Serial.print(motor_index, DEC);
      Serial.print(" is up and running \n");
    } else {
      Serial.print("TMC2209 # ");
      Serial.print(motor_index, DEC);
      Serial.print(" is not communicating \n");
    }
  }

  stepper_controller.setup(CHIP_SELECT_PIN,CLOCK_FREQUENCY_MHZ); // setup TMC429
  stepper_controller.initialize();  // initialize TMC429
  if (stepper_controller.communicating()) // if communicating
  {
    Serial.println("TMC429 is up and communicating");
    Serial.println("Homing procedure starting...");
  } else {
    Serial.println("TMC429 is not communicating");
  }

  /* TMC2209 Setup */
  for (size_t motor_index = 0; motor_index < MOTOR_COUNT; ++motor_index)
  {
    TMC2209 & stepper_driver = stepper_drivers[motor_index]; // setup alias for  object in object array
    stepper_driver.disableInverseMotorDirection(); // disable inverse motor direction GCONF flag
    stepper_driver.setRunCurrent(RUN_CURRENT);   // run current setup
    stepper_driver.setHoldCurrent(HOLD_CURRENT); // hold current setup
    stepper_driver.setHoldDelay(HOLD_DELAY); // hold delay setup
    stepper_driver.setMicrostepsPerStep(MICROSTEPS_PER_STEP); // microstepping
    stepper_driver.setStandstillMode(STANDSTILL_MODE); // standstill setup
    stepper_driver.setPwmOffset(PWM_OFFSET); 
    stepper_driver.setPwmGradient(PWM_GRADIENT);
    stepper_driver.disableAutomaticCurrentScaling();
    stepper_driver.disableAutomaticGradientAdaptation();
    stepper_driver.setCoolStepDurationThreshold(COOL_STEP_DURATION_THRESHOLD);
    stepper_driver.setCoolStepCurrentIncrement(COOL_STEP_CURRENT_INCREMENT);
    stepper_driver.enableCoolStep(COOL_STEP_LOWER_THRESHOLD,COOL_STEP_UPPER_THRESHOLD);
  }

  /* TMC429 Setup */
  /* Left reference stop for motor 1 (X)
     Right reference stop for motor 2 (Y) */
  stepper_controller.disableInverseStepPolarity(); // disables active low for STEP/DIR
  stepper_controller.disableInverseDirPolarity();
  stepper_controller.setSwitchesActiveLow(); // the stop switches are active low - pass thru NOT gate
  stepper_controller.setReferenceSwitchToLeft(X_MOTOR);
  stepper_controller.setReferenceSwitchToRight(Y_MOTOR);
  stepper_controller.disableRightSwitchStop(X_MOTOR); // Right/left switch doesn't exist
  stepper_controller.disableLeftSwitchStop(Y_MOTOR);
  for (size_t motor_index=0; motor_index < MOTOR_COUNT; ++motor_index)
  {
    stepper_controller.disableSwitchSoftStop(motor_index); // stop immediately upon stall
    stepper_controller.setRampMode(motor_index); // ramp mode for internal uStep plane
    stepper_controller.setLimitsInHz(motor_index,VELOCITY_MIN,VELOCITY_MAX,ACCELERATION_MAX);
  }
  
  
  /* Enable Drivers */
  for (size_t motor_index=0; motor_index<MOTOR_COUNT; ++motor_index)
  {
    TMC2209 & stepper_driver = stepper_drivers[motor_index];
    stepper_driver.enable();
  }

  homing(); // run the homing procedure, construct plane within TMC429
}

void loop()
{
  Bus.send(test);
  test++;
}

/* Homing procedure, assume TMC2209 StallGuard output as limit switch */
void homing()
{
  // first move the motors out of the axis minimums (if they are in)
  stepper_controller.setTargetPosition(X_MOTOR, MICROSTEPS_PER_QUARTER_REV); // move to the right by 1 rev
  stepper_controller.setTargetPosition(Y_MOTOR, -MICROSTEPS_PER_QUARTER_REV); // move to the top ^
  
  // ************ Start homing procedure, acquire ORIGIN POSITION *************
  for (size_t motor_index = 0; motor_index < MOTOR_COUNT; ++motor_index)
  {
    // 1. Initialize X_LATCHED register
    stepper_controller.latchPositionWaiting(motor_index);

    // 2. Run the motors back to origin point. Once stall is received, the location is latched.
    switch (motor_index)
    {
      case X_MOTOR:
        // Ram the motor to the largest possible int32 value
        stepper_controller.setTargetPosition(X_MOTOR, 0x7FFFFFFF);
        break;
      case Y_MOTOR:
        // Ram the motor to the smallest possible int32 value
        stepper_controller.setTargetPosition(Y_MOTOR, 0x80000000);
        break;
      default:
        break;
    }

    // 3. Stepper motor automatically stops upon stall. Move away by 1 full step to avoid origin stall
    switch (motor_index)
    {
      case X_MOTOR:
        // rotate 256 micro steps (1 full step)
        stepper_controller.setActualPosition(motor_index, 0);
        stepper_controller.setTargetPosition(X_MOTOR, 256);
        break;
      case Y_MOTOR:
        // rotate -256 micro steps
        stepper_controller.setActualPosition(motor_index, 0);
        stepper_controller.setTargetPosition(Y_MOTOR, -256);
        break;
      default:
        break;
    }

    // 4. Reset TMC429 reference point
    stepper_controller.setActualPosition(motor_index, 0);
    stepper_controller.setTargetPosition(motor_index, 0);
  }

  // ************ Continue homing procedure, acquire MAXIMUM POSITION *************
  // Attach intterupt to stall detection for maximum detection
  attachInterrupt(digitalPinToInterrupt(X_ERROR), stall_X, FALLING);
  attachInterrupt(digitalPinToInterrupt(Y_ERROR), stall_Y, FALLING);

  // begin moving stage to absolute maximum
  stepper_controller.setTargetPosition(X_MOTOR, 0x7FFFFFFF);
  stepper_controller.setTargetPosition(Y_MOTOR, 0x80000000);

  while(1)
  {
    if (X_triggered) 
    {
      stepper_controller.stop(X_MOTOR); // stop X motor, which sets it into speed mode
      X_max = stepper_controller.getActualPosition(X_MOTOR) - 256; // acquire max coordinate
      stepper_controller.setSoftMode(X_MOTOR); // set soft decelerate
      X_triggered = false;
    }

    if (Y_triggered) 
    {
      stepper_controller.stop(Y_MOTOR); // stop Y motor 
      Y_max = stepper_controller.getActualPosition(Y_MOTOR) - 256; // acquire max coordinate
      stepper_controller.setSoftMode(Y_MOTOR); // set soft decelerate
      accumulator++;
      Y_triggered = false;
    }

    if (accumulator == 2) // both triggered, leave
    break;
  }

  // detach interrupt
  detachInterrupt(digitalPinToInterrupt(X_ERROR));
  detachInterrupt(digitalPinToInterrupt(Y_ERROR));

  // ************ Complete homing procedure, go to origin point. *************
  stepper_controller.setTargetPosition(X_MOTOR, 0);
  stepper_controller.setTargetPosition(Y_MOTOR, 0);
}

/* Interrupt handler for X and Y stall, used in homing only */
void stall_X()
{
  X_triggered = true;
  accumulator++;
}

void stall_Y()
{
  Y_triggered = true;
  accumulator++;
}