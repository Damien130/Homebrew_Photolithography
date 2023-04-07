#line 1 "/Users/damienhu/Documents/Git/Homebrew_Photolithography/src/StageController/Build/preproc/ctags_target_for_gcc_minus_e.cpp"
# 1 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
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
# 19 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
# 20 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 2
# 21 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 2
# 22 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 2
# 23 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 2
# 24 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 2

# 26 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 2


# 27 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
/* #include "NeuBus.h"

 */
/* ATMega2560 Pin Mapping */
/* Digital Pins:

  RESET: RESET pin for emergency stop

  2: TMC2209_X error flag (PE4, 6) Interrupt 5 

  3: TMC2209_Y error flag (PE5, 7) Interrupt 6

  49: TMC429 Chip Select (PL0, 35)

*/
# 37 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
const static int8_t nWAIT = 26;
const static int8_t INIR = 27;
const static int8_t nRESET = 28;
const static int8_t nWRITE = 29;
const static int8_t RDY = 24;
const static int8_t EXPRDY = 25;
const static int8_t nDATASTRB = 2;
const static int8_t nADDRSTRB = 3;
const static int8_t X_ERROR = 6;
const static int8_t Y_ERROR = 7;
const static int8_t CHIP_SELECT_PIN = 8;
const static int8_t I2CADDR = 0x0F;
const long SERIAL_BAUD_RATE = 115200;

/* Data structure

Byte1: scribble: magic number

Byte2: Command

Byte3-6: data (4 bytes)

Byte7: 0x00

Byte8: Chksum 

 */
# 58 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
enum Flags
{ MOTOR_COUNT = 2,
  X_MOTOR = 1,
  Y_MOTOR = 2
};
int32_t X_target = 0, X_actual = 0, Y_target = 0, Y_actual = 0, X_command = 0, Y_command = 0; // plane location, initialize to 0
int32_t X_latched = 0, Y_latched = 0;
bool stageHalt = false;
bool calibration = false;
bool calibrationComplete = false;
int8_t query = 0;


/* TMC2209 driver settings ==================================================================*/
/* Note: Current TMC2209 setup doesn't utilize UART address for simplicity of troubleshooting

   Instead, UART channel 1 and 2 is used for separate controllers 

   NOT gate required for output of TMC2209 error detection */
# 75 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
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
# 96 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
const int CLOCK_FREQUENCY_MHZ = 16; // clock for step frequency
const int STEPS_PER_REV = 200; // 1.8 degrees per step
const int REVS_PER_SEC_MAX = 1; // max revolution per seconds
const int ACCELERATION_MAX = 50000/2; // 50 kHz, accelerate from 0 to max
const int MICROSTEPS_PER_REV = STEPS_PER_REV * MICROSTEPS_PER_STEP;
const long VELOCITY_MAX = MICROSTEPS_PER_REV * REVS_PER_SEC_MAX;
const long VELOCITY_MIN = 100;
const long AXIS_LENGTH = 200000000; // 2e8 nm (200mm) per axis
const long LENGTH_PER_REV = 5000000; // 5e6 nm (5mm) per revolution
const long REV_PER_AXIS = AXIS_LENGTH / LENGTH_PER_REV; // get revolutions per axis
int32_t MICROSTEPS_PER_AXIS = REV_PER_AXIS * MICROSTEPS_PER_REV; // get maximum microsteps per axis
// const long DISTANCE_PER_MICROSTEP = LENGTH_PER_REV / MICROSTEPS_PER_REV; 
// 0.097 micron per step, float (4 bytes)

/* Instantiate stepper drive train*/
TMC2209 stepper_drivers[MOTOR_COUNT]; // TMC2209, setup as object array
TMC429 stepper_controller; // TMC429 Stepper Controller
TMC429::Status status; // TMC429 status struct



// INSTRUCTION HANDLER FUNCTIONS AND INITIALIZATION



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
  Serial.begin(SERIAL_BAUD_RATE); // Serial console on serial0 @ 15200 baud
  Wire.begin(I2CADDR); // setup as slave with address I2CADDR
  Wire.onRequest(requestEvent); // request event
  Wire.onReceive(receiveEvent); // receive event


  /* ATMega2560 Setup */
  if (__builtin_constant_p(X_ERROR) && __builtin_constant_p(0x0)) { if (0x0 == 0x2) { ((0x0) ? ((*(((X_ERROR) >= 22 && (X_ERROR) <= 29) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0X01) + 0x20)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 10 && (X_ERROR) <= 13) || ((X_ERROR) >= 50 && (X_ERROR) <= 53)) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x04) + 0x20)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) >= 30 && (X_ERROR) <= 37) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x07) + 0x20)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 18 && (X_ERROR) <= 21) || (X_ERROR) == 38) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0A) + 0x20)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 0 && (X_ERROR) <= 3) || (X_ERROR) == 5) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0D) + 0x20)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) >= 54 && (X_ERROR) <= 61) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x10) + 0x20)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 39 && (X_ERROR) <= 41) || (X_ERROR) == 4) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x13) + 0x20)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 6 && (X_ERROR) <= 9) || (X_ERROR) == 16 || (X_ERROR) == 17) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x101)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) == 14 || (X_ERROR) == 15) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x104)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) >= 62 && (X_ERROR) <= 69) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x107)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x10A))
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 ))))))))))) |= (1UL << ((((X_ERROR) >= 7 && (X_ERROR) <= 9) ? (X_ERROR) - 3 : (((X_ERROR) >= 10 && (X_ERROR) <= 13) ? (X_ERROR) - 6 : (((X_ERROR) >= 22 && (X_ERROR) <= 29) ? (X_ERROR) - 22 : (((X_ERROR) >= 30 && (X_ERROR) <= 37) ? 37 - (X_ERROR) : (((X_ERROR) >= 39 && (X_ERROR) <= 41) ? 41 - (X_ERROR) : (((X_ERROR) >= 42 && (X_ERROR) <= 49) ? 49 - (X_ERROR) : (((X_ERROR) >= 50 && (X_ERROR) <= 53) ? 53 - (X_ERROR) : (((X_ERROR) >= 54 && (X_ERROR) <= 61) ? (X_ERROR) - 54 : (((X_ERROR) >= 62 && (X_ERROR) <= 69) ? (X_ERROR) - 62 : (((X_ERROR) == 0 || (X_ERROR) == 15 || (X_ERROR) == 17 || (X_ERROR) == 21) ? 0 : (((X_ERROR) == 1 || (X_ERROR) == 14 || (X_ERROR) == 16 || (X_ERROR) == 20) ? 1 : (((X_ERROR) == 19) ? 2 : (((X_ERROR) == 5 || (X_ERROR) == 6 || (X_ERROR) == 18) ? 3 : (((X_ERROR) == 2) ? 4 : (((X_ERROR) == 3 || (X_ERROR) == 4) ? 5 : 7)))))))))))))))))) : ((*(((X_ERROR) >= 22 && (X_ERROR) <= 29) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0X01) + 0x20)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 10 && (X_ERROR) <= 13) || ((X_ERROR) >= 50 && (X_ERROR) <= 53)) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x04) + 0x20)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) >= 30 && (X_ERROR) <= 37) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x07) + 0x20)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 18 && (X_ERROR) <= 21) || (X_ERROR) == 38) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0A) + 0x20)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 0 && (X_ERROR) <= 3) || (X_ERROR) == 5) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0D) + 0x20)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) >= 54 && (X_ERROR) <= 61) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x10) + 0x20)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 39 && (X_ERROR) <= 41) || (X_ERROR) == 4) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x13) + 0x20)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 6 && (X_ERROR) <= 9) || (X_ERROR) == 16 || (X_ERROR) == 17) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x101)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) == 14 || (X_ERROR) == 15) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x104)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) >= 62 && (X_ERROR) <= 69) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x107)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x10A))
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 ))))))))))) &= ~(1UL << ((((X_ERROR) >= 7 && (X_ERROR) <= 9) ? (X_ERROR) - 3 : (((X_ERROR) >= 10 && (X_ERROR) <= 13) ? (X_ERROR) - 6 : (((X_ERROR) >= 22 && (X_ERROR) <= 29) ? (X_ERROR) - 22 : (((X_ERROR) >= 30 && (X_ERROR) <= 37) ? 37 - (X_ERROR) : (((X_ERROR) >= 39 && (X_ERROR) <= 41) ? 41 - (X_ERROR) : (((X_ERROR) >= 42 && (X_ERROR) <= 49) ? 49 - (X_ERROR) : (((X_ERROR) >= 50 && (X_ERROR) <= 53) ? 53 - (X_ERROR) : (((X_ERROR) >= 54 && (X_ERROR) <= 61) ? (X_ERROR) - 54 : (((X_ERROR) >= 62 && (X_ERROR) <= 69) ? (X_ERROR) - 62 : (((X_ERROR) == 0 || (X_ERROR) == 15 || (X_ERROR) == 17 || (X_ERROR) == 21) ? 0 : (((X_ERROR) == 1 || (X_ERROR) == 14 || (X_ERROR) == 16 || (X_ERROR) == 20) ? 1 : (((X_ERROR) == 19) ? 2 : (((X_ERROR) == 5 || (X_ERROR) == 6 || (X_ERROR) == 18) ? 3 : (((X_ERROR) == 2) ? 4 : (((X_ERROR) == 3 || (X_ERROR) == 4) ? 5 : 7))))))))))))))))))); ((0x1) ? ((*(((X_ERROR) >= 22 && (X_ERROR) <= 29) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0X02) + 0x20)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 10 && (X_ERROR) <= 13) || ((X_ERROR) >= 50 && (X_ERROR) <= 53)) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x05) + 0x20)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) >= 30 && (X_ERROR) <= 37) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x08) + 0x20)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 18 && (X_ERROR) <= 21) || (X_ERROR) == 38) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0B) + 0x20)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 0 && (X_ERROR) <= 3) || (X_ERROR) == 5) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0E) + 0x20)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) >= 54 && (X_ERROR) <= 61) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x11) + 0x20)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 39 && (X_ERROR) <= 41) || (X_ERROR) == 4) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x14) + 0x20)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 6 && (X_ERROR) <= 9) || (X_ERROR) == 16 || (X_ERROR) == 17) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x102)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) == 14 || (X_ERROR) == 15) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x105)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) >= 62 && (X_ERROR) <= 69) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x108)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x10B))
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 ))))))))))) |= (1UL << ((((X_ERROR) >= 7 && (X_ERROR) <= 9) ? (X_ERROR) - 3 : (((X_ERROR) >= 10 && (X_ERROR) <= 13) ? (X_ERROR) - 6 : (((X_ERROR) >= 22 && (X_ERROR) <= 29) ? (X_ERROR) - 22 : (((X_ERROR) >= 30 && (X_ERROR) <= 37) ? 37 - (X_ERROR) : (((X_ERROR) >= 39 && (X_ERROR) <= 41) ? 41 - (X_ERROR) : (((X_ERROR) >= 42 && (X_ERROR) <= 49) ? 49 - (X_ERROR) : (((X_ERROR) >= 50 && (X_ERROR) <= 53) ? 53 - (X_ERROR) : (((X_ERROR) >= 54 && (X_ERROR) <= 61) ? (X_ERROR) - 54 : (((X_ERROR) >= 62 && (X_ERROR) <= 69) ? (X_ERROR) - 62 : (((X_ERROR) == 0 || (X_ERROR) == 15 || (X_ERROR) == 17 || (X_ERROR) == 21) ? 0 : (((X_ERROR) == 1 || (X_ERROR) == 14 || (X_ERROR) == 16 || (X_ERROR) == 20) ? 1 : (((X_ERROR) == 19) ? 2 : (((X_ERROR) == 5 || (X_ERROR) == 6 || (X_ERROR) == 18) ? 3 : (((X_ERROR) == 2) ? 4 : (((X_ERROR) == 3 || (X_ERROR) == 4) ? 5 : 7)))))))))))))))))) : ((*(((X_ERROR) >= 22 && (X_ERROR) <= 29) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0X02) + 0x20)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 10 && (X_ERROR) <= 13) || ((X_ERROR) >= 50 && (X_ERROR) <= 53)) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x05) + 0x20)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) >= 30 && (X_ERROR) <= 37) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x08) + 0x20)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 18 && (X_ERROR) <= 21) || (X_ERROR) == 38) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0B) + 0x20)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 0 && (X_ERROR) <= 3) || (X_ERROR) == 5) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0E) + 0x20)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) >= 54 && (X_ERROR) <= 61) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x11) + 0x20)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 39 && (X_ERROR) <= 41) || (X_ERROR) == 4) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x14) + 0x20)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 6 && (X_ERROR) <= 9) || (X_ERROR) == 16 || (X_ERROR) == 17) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x102)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) == 14 || (X_ERROR) == 15) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x105)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) >= 62 && (X_ERROR) <= 69) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x108)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x10B))
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 ))))))))))) &= ~(1UL << ((((X_ERROR) >= 7 && (X_ERROR) <= 9) ? (X_ERROR) - 3 : (((X_ERROR) >= 10 && (X_ERROR) <= 13) ? (X_ERROR) - 6 : (((X_ERROR) >= 22 && (X_ERROR) <= 29) ? (X_ERROR) - 22 : (((X_ERROR) >= 30 && (X_ERROR) <= 37) ? 37 - (X_ERROR) : (((X_ERROR) >= 39 && (X_ERROR) <= 41) ? 41 - (X_ERROR) : (((X_ERROR) >= 42 && (X_ERROR) <= 49) ? 49 - (X_ERROR) : (((X_ERROR) >= 50 && (X_ERROR) <= 53) ? 53 - (X_ERROR) : (((X_ERROR) >= 54 && (X_ERROR) <= 61) ? (X_ERROR) - 54 : (((X_ERROR) >= 62 && (X_ERROR) <= 69) ? (X_ERROR) - 62 : (((X_ERROR) == 0 || (X_ERROR) == 15 || (X_ERROR) == 17 || (X_ERROR) == 21) ? 0 : (((X_ERROR) == 1 || (X_ERROR) == 14 || (X_ERROR) == 16 || (X_ERROR) == 20) ? 1 : (((X_ERROR) == 19) ? 2 : (((X_ERROR) == 5 || (X_ERROR) == 6 || (X_ERROR) == 18) ? 3 : (((X_ERROR) == 2) ? 4 : (((X_ERROR) == 3 || (X_ERROR) == 4) ? 5 : 7))))))))))))))))))); } else { ((0x0) ? ((*(((X_ERROR) >= 22 && (X_ERROR) <= 29) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0X01) + 0x20)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 10 && (X_ERROR) <= 13) || ((X_ERROR) >= 50 && (X_ERROR) <= 53)) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x04) + 0x20)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) >= 30 && (X_ERROR) <= 37) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x07) + 0x20)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 18 && (X_ERROR) <= 21) || (X_ERROR) == 38) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0A) + 0x20)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 0 && (X_ERROR) <= 3) || (X_ERROR) == 5) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0D) + 0x20)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) >= 54 && (X_ERROR) <= 61) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x10) + 0x20)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 39 && (X_ERROR) <= 41) || (X_ERROR) == 4) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x13) + 0x20)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 6 && (X_ERROR) <= 9) || (X_ERROR) == 16 || (X_ERROR) == 17) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x101)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) == 14 || (X_ERROR) == 15) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x104)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) >= 62 && (X_ERROR) <= 69) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x107)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x10A))
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 ))))))))))) |= (1UL << ((((X_ERROR) >= 7 && (X_ERROR) <= 9) ? (X_ERROR) - 3 : (((X_ERROR) >= 10 && (X_ERROR) <= 13) ? (X_ERROR) - 6 : (((X_ERROR) >= 22 && (X_ERROR) <= 29) ? (X_ERROR) - 22 : (((X_ERROR) >= 30 && (X_ERROR) <= 37) ? 37 - (X_ERROR) : (((X_ERROR) >= 39 && (X_ERROR) <= 41) ? 41 - (X_ERROR) : (((X_ERROR) >= 42 && (X_ERROR) <= 49) ? 49 - (X_ERROR) : (((X_ERROR) >= 50 && (X_ERROR) <= 53) ? 53 - (X_ERROR) : (((X_ERROR) >= 54 && (X_ERROR) <= 61) ? (X_ERROR) - 54 : (((X_ERROR) >= 62 && (X_ERROR) <= 69) ? (X_ERROR) - 62 : (((X_ERROR) == 0 || (X_ERROR) == 15 || (X_ERROR) == 17 || (X_ERROR) == 21) ? 0 : (((X_ERROR) == 1 || (X_ERROR) == 14 || (X_ERROR) == 16 || (X_ERROR) == 20) ? 1 : (((X_ERROR) == 19) ? 2 : (((X_ERROR) == 5 || (X_ERROR) == 6 || (X_ERROR) == 18) ? 3 : (((X_ERROR) == 2) ? 4 : (((X_ERROR) == 3 || (X_ERROR) == 4) ? 5 : 7)))))))))))))))))) : ((*(((X_ERROR) >= 22 && (X_ERROR) <= 29) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0X01) + 0x20)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 10 && (X_ERROR) <= 13) || ((X_ERROR) >= 50 && (X_ERROR) <= 53)) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x04) + 0x20)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) >= 30 && (X_ERROR) <= 37) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x07) + 0x20)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 18 && (X_ERROR) <= 21) || (X_ERROR) == 38) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0A) + 0x20)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 0 && (X_ERROR) <= 3) || (X_ERROR) == 5) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0D) + 0x20)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) >= 54 && (X_ERROR) <= 61) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x10) + 0x20)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 39 && (X_ERROR) <= 41) || (X_ERROR) == 4) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x13) + 0x20)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 6 && (X_ERROR) <= 9) || (X_ERROR) == 16 || (X_ERROR) == 17) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x101)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) == 14 || (X_ERROR) == 15) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x104)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) >= 62 && (X_ERROR) <= 69) ? &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x107)) 
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : &
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x10A))
# 161 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 ))))))))))) &= ~(1UL << ((((X_ERROR) >= 7 && (X_ERROR) <= 9) ? (X_ERROR) - 3 : (((X_ERROR) >= 10 && (X_ERROR) <= 13) ? (X_ERROR) - 6 : (((X_ERROR) >= 22 && (X_ERROR) <= 29) ? (X_ERROR) - 22 : (((X_ERROR) >= 30 && (X_ERROR) <= 37) ? 37 - (X_ERROR) : (((X_ERROR) >= 39 && (X_ERROR) <= 41) ? 41 - (X_ERROR) : (((X_ERROR) >= 42 && (X_ERROR) <= 49) ? 49 - (X_ERROR) : (((X_ERROR) >= 50 && (X_ERROR) <= 53) ? 53 - (X_ERROR) : (((X_ERROR) >= 54 && (X_ERROR) <= 61) ? (X_ERROR) - 54 : (((X_ERROR) >= 62 && (X_ERROR) <= 69) ? (X_ERROR) - 62 : (((X_ERROR) == 0 || (X_ERROR) == 15 || (X_ERROR) == 17 || (X_ERROR) == 21) ? 0 : (((X_ERROR) == 1 || (X_ERROR) == 14 || (X_ERROR) == 16 || (X_ERROR) == 20) ? 1 : (((X_ERROR) == 19) ? 2 : (((X_ERROR) == 5 || (X_ERROR) == 6 || (X_ERROR) == 18) ? 3 : (((X_ERROR) == 2) ? 4 : (((X_ERROR) == 3 || (X_ERROR) == 4) ? 5 : 7))))))))))))))))))); } } else { pinMode((X_ERROR), (0x0)); };
  if (__builtin_constant_p(Y_ERROR) && __builtin_constant_p(0x0)) { if (0x0 == 0x2) { ((0x0) ? ((*(((Y_ERROR) >= 22 && (Y_ERROR) <= 29) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0X01) + 0x20)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 10 && (Y_ERROR) <= 13) || ((Y_ERROR) >= 50 && (Y_ERROR) <= 53)) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x04) + 0x20)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) >= 30 && (Y_ERROR) <= 37) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x07) + 0x20)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 18 && (Y_ERROR) <= 21) || (Y_ERROR) == 38) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0A) + 0x20)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 0 && (Y_ERROR) <= 3) || (Y_ERROR) == 5) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0D) + 0x20)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) >= 54 && (Y_ERROR) <= 61) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x10) + 0x20)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 39 && (Y_ERROR) <= 41) || (Y_ERROR) == 4) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x13) + 0x20)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 6 && (Y_ERROR) <= 9) || (Y_ERROR) == 16 || (Y_ERROR) == 17) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x101)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) == 14 || (Y_ERROR) == 15) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x104)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) >= 62 && (Y_ERROR) <= 69) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x107)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x10A))
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 ))))))))))) |= (1UL << ((((Y_ERROR) >= 7 && (Y_ERROR) <= 9) ? (Y_ERROR) - 3 : (((Y_ERROR) >= 10 && (Y_ERROR) <= 13) ? (Y_ERROR) - 6 : (((Y_ERROR) >= 22 && (Y_ERROR) <= 29) ? (Y_ERROR) - 22 : (((Y_ERROR) >= 30 && (Y_ERROR) <= 37) ? 37 - (Y_ERROR) : (((Y_ERROR) >= 39 && (Y_ERROR) <= 41) ? 41 - (Y_ERROR) : (((Y_ERROR) >= 42 && (Y_ERROR) <= 49) ? 49 - (Y_ERROR) : (((Y_ERROR) >= 50 && (Y_ERROR) <= 53) ? 53 - (Y_ERROR) : (((Y_ERROR) >= 54 && (Y_ERROR) <= 61) ? (Y_ERROR) - 54 : (((Y_ERROR) >= 62 && (Y_ERROR) <= 69) ? (Y_ERROR) - 62 : (((Y_ERROR) == 0 || (Y_ERROR) == 15 || (Y_ERROR) == 17 || (Y_ERROR) == 21) ? 0 : (((Y_ERROR) == 1 || (Y_ERROR) == 14 || (Y_ERROR) == 16 || (Y_ERROR) == 20) ? 1 : (((Y_ERROR) == 19) ? 2 : (((Y_ERROR) == 5 || (Y_ERROR) == 6 || (Y_ERROR) == 18) ? 3 : (((Y_ERROR) == 2) ? 4 : (((Y_ERROR) == 3 || (Y_ERROR) == 4) ? 5 : 7)))))))))))))))))) : ((*(((Y_ERROR) >= 22 && (Y_ERROR) <= 29) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0X01) + 0x20)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 10 && (Y_ERROR) <= 13) || ((Y_ERROR) >= 50 && (Y_ERROR) <= 53)) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x04) + 0x20)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) >= 30 && (Y_ERROR) <= 37) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x07) + 0x20)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 18 && (Y_ERROR) <= 21) || (Y_ERROR) == 38) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0A) + 0x20)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 0 && (Y_ERROR) <= 3) || (Y_ERROR) == 5) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0D) + 0x20)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) >= 54 && (Y_ERROR) <= 61) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x10) + 0x20)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 39 && (Y_ERROR) <= 41) || (Y_ERROR) == 4) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x13) + 0x20)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 6 && (Y_ERROR) <= 9) || (Y_ERROR) == 16 || (Y_ERROR) == 17) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x101)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) == 14 || (Y_ERROR) == 15) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x104)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) >= 62 && (Y_ERROR) <= 69) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x107)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x10A))
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 ))))))))))) &= ~(1UL << ((((Y_ERROR) >= 7 && (Y_ERROR) <= 9) ? (Y_ERROR) - 3 : (((Y_ERROR) >= 10 && (Y_ERROR) <= 13) ? (Y_ERROR) - 6 : (((Y_ERROR) >= 22 && (Y_ERROR) <= 29) ? (Y_ERROR) - 22 : (((Y_ERROR) >= 30 && (Y_ERROR) <= 37) ? 37 - (Y_ERROR) : (((Y_ERROR) >= 39 && (Y_ERROR) <= 41) ? 41 - (Y_ERROR) : (((Y_ERROR) >= 42 && (Y_ERROR) <= 49) ? 49 - (Y_ERROR) : (((Y_ERROR) >= 50 && (Y_ERROR) <= 53) ? 53 - (Y_ERROR) : (((Y_ERROR) >= 54 && (Y_ERROR) <= 61) ? (Y_ERROR) - 54 : (((Y_ERROR) >= 62 && (Y_ERROR) <= 69) ? (Y_ERROR) - 62 : (((Y_ERROR) == 0 || (Y_ERROR) == 15 || (Y_ERROR) == 17 || (Y_ERROR) == 21) ? 0 : (((Y_ERROR) == 1 || (Y_ERROR) == 14 || (Y_ERROR) == 16 || (Y_ERROR) == 20) ? 1 : (((Y_ERROR) == 19) ? 2 : (((Y_ERROR) == 5 || (Y_ERROR) == 6 || (Y_ERROR) == 18) ? 3 : (((Y_ERROR) == 2) ? 4 : (((Y_ERROR) == 3 || (Y_ERROR) == 4) ? 5 : 7))))))))))))))))))); ((0x1) ? ((*(((Y_ERROR) >= 22 && (Y_ERROR) <= 29) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0X02) + 0x20)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 10 && (Y_ERROR) <= 13) || ((Y_ERROR) >= 50 && (Y_ERROR) <= 53)) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x05) + 0x20)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) >= 30 && (Y_ERROR) <= 37) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x08) + 0x20)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 18 && (Y_ERROR) <= 21) || (Y_ERROR) == 38) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0B) + 0x20)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 0 && (Y_ERROR) <= 3) || (Y_ERROR) == 5) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0E) + 0x20)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) >= 54 && (Y_ERROR) <= 61) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x11) + 0x20)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 39 && (Y_ERROR) <= 41) || (Y_ERROR) == 4) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x14) + 0x20)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 6 && (Y_ERROR) <= 9) || (Y_ERROR) == 16 || (Y_ERROR) == 17) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x102)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) == 14 || (Y_ERROR) == 15) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x105)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) >= 62 && (Y_ERROR) <= 69) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x108)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x10B))
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 ))))))))))) |= (1UL << ((((Y_ERROR) >= 7 && (Y_ERROR) <= 9) ? (Y_ERROR) - 3 : (((Y_ERROR) >= 10 && (Y_ERROR) <= 13) ? (Y_ERROR) - 6 : (((Y_ERROR) >= 22 && (Y_ERROR) <= 29) ? (Y_ERROR) - 22 : (((Y_ERROR) >= 30 && (Y_ERROR) <= 37) ? 37 - (Y_ERROR) : (((Y_ERROR) >= 39 && (Y_ERROR) <= 41) ? 41 - (Y_ERROR) : (((Y_ERROR) >= 42 && (Y_ERROR) <= 49) ? 49 - (Y_ERROR) : (((Y_ERROR) >= 50 && (Y_ERROR) <= 53) ? 53 - (Y_ERROR) : (((Y_ERROR) >= 54 && (Y_ERROR) <= 61) ? (Y_ERROR) - 54 : (((Y_ERROR) >= 62 && (Y_ERROR) <= 69) ? (Y_ERROR) - 62 : (((Y_ERROR) == 0 || (Y_ERROR) == 15 || (Y_ERROR) == 17 || (Y_ERROR) == 21) ? 0 : (((Y_ERROR) == 1 || (Y_ERROR) == 14 || (Y_ERROR) == 16 || (Y_ERROR) == 20) ? 1 : (((Y_ERROR) == 19) ? 2 : (((Y_ERROR) == 5 || (Y_ERROR) == 6 || (Y_ERROR) == 18) ? 3 : (((Y_ERROR) == 2) ? 4 : (((Y_ERROR) == 3 || (Y_ERROR) == 4) ? 5 : 7)))))))))))))))))) : ((*(((Y_ERROR) >= 22 && (Y_ERROR) <= 29) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0X02) + 0x20)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 10 && (Y_ERROR) <= 13) || ((Y_ERROR) >= 50 && (Y_ERROR) <= 53)) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x05) + 0x20)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) >= 30 && (Y_ERROR) <= 37) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x08) + 0x20)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 18 && (Y_ERROR) <= 21) || (Y_ERROR) == 38) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0B) + 0x20)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 0 && (Y_ERROR) <= 3) || (Y_ERROR) == 5) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0E) + 0x20)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) >= 54 && (Y_ERROR) <= 61) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x11) + 0x20)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 39 && (Y_ERROR) <= 41) || (Y_ERROR) == 4) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x14) + 0x20)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 6 && (Y_ERROR) <= 9) || (Y_ERROR) == 16 || (Y_ERROR) == 17) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x102)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) == 14 || (Y_ERROR) == 15) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x105)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) >= 62 && (Y_ERROR) <= 69) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x108)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x10B))
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 ))))))))))) &= ~(1UL << ((((Y_ERROR) >= 7 && (Y_ERROR) <= 9) ? (Y_ERROR) - 3 : (((Y_ERROR) >= 10 && (Y_ERROR) <= 13) ? (Y_ERROR) - 6 : (((Y_ERROR) >= 22 && (Y_ERROR) <= 29) ? (Y_ERROR) - 22 : (((Y_ERROR) >= 30 && (Y_ERROR) <= 37) ? 37 - (Y_ERROR) : (((Y_ERROR) >= 39 && (Y_ERROR) <= 41) ? 41 - (Y_ERROR) : (((Y_ERROR) >= 42 && (Y_ERROR) <= 49) ? 49 - (Y_ERROR) : (((Y_ERROR) >= 50 && (Y_ERROR) <= 53) ? 53 - (Y_ERROR) : (((Y_ERROR) >= 54 && (Y_ERROR) <= 61) ? (Y_ERROR) - 54 : (((Y_ERROR) >= 62 && (Y_ERROR) <= 69) ? (Y_ERROR) - 62 : (((Y_ERROR) == 0 || (Y_ERROR) == 15 || (Y_ERROR) == 17 || (Y_ERROR) == 21) ? 0 : (((Y_ERROR) == 1 || (Y_ERROR) == 14 || (Y_ERROR) == 16 || (Y_ERROR) == 20) ? 1 : (((Y_ERROR) == 19) ? 2 : (((Y_ERROR) == 5 || (Y_ERROR) == 6 || (Y_ERROR) == 18) ? 3 : (((Y_ERROR) == 2) ? 4 : (((Y_ERROR) == 3 || (Y_ERROR) == 4) ? 5 : 7))))))))))))))))))); } else { ((0x0) ? ((*(((Y_ERROR) >= 22 && (Y_ERROR) <= 29) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0X01) + 0x20)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 10 && (Y_ERROR) <= 13) || ((Y_ERROR) >= 50 && (Y_ERROR) <= 53)) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x04) + 0x20)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) >= 30 && (Y_ERROR) <= 37) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x07) + 0x20)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 18 && (Y_ERROR) <= 21) || (Y_ERROR) == 38) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0A) + 0x20)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 0 && (Y_ERROR) <= 3) || (Y_ERROR) == 5) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0D) + 0x20)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) >= 54 && (Y_ERROR) <= 61) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x10) + 0x20)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 39 && (Y_ERROR) <= 41) || (Y_ERROR) == 4) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x13) + 0x20)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 6 && (Y_ERROR) <= 9) || (Y_ERROR) == 16 || (Y_ERROR) == 17) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x101)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) == 14 || (Y_ERROR) == 15) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x104)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) >= 62 && (Y_ERROR) <= 69) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x107)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x10A))
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 ))))))))))) |= (1UL << ((((Y_ERROR) >= 7 && (Y_ERROR) <= 9) ? (Y_ERROR) - 3 : (((Y_ERROR) >= 10 && (Y_ERROR) <= 13) ? (Y_ERROR) - 6 : (((Y_ERROR) >= 22 && (Y_ERROR) <= 29) ? (Y_ERROR) - 22 : (((Y_ERROR) >= 30 && (Y_ERROR) <= 37) ? 37 - (Y_ERROR) : (((Y_ERROR) >= 39 && (Y_ERROR) <= 41) ? 41 - (Y_ERROR) : (((Y_ERROR) >= 42 && (Y_ERROR) <= 49) ? 49 - (Y_ERROR) : (((Y_ERROR) >= 50 && (Y_ERROR) <= 53) ? 53 - (Y_ERROR) : (((Y_ERROR) >= 54 && (Y_ERROR) <= 61) ? (Y_ERROR) - 54 : (((Y_ERROR) >= 62 && (Y_ERROR) <= 69) ? (Y_ERROR) - 62 : (((Y_ERROR) == 0 || (Y_ERROR) == 15 || (Y_ERROR) == 17 || (Y_ERROR) == 21) ? 0 : (((Y_ERROR) == 1 || (Y_ERROR) == 14 || (Y_ERROR) == 16 || (Y_ERROR) == 20) ? 1 : (((Y_ERROR) == 19) ? 2 : (((Y_ERROR) == 5 || (Y_ERROR) == 6 || (Y_ERROR) == 18) ? 3 : (((Y_ERROR) == 2) ? 4 : (((Y_ERROR) == 3 || (Y_ERROR) == 4) ? 5 : 7)))))))))))))))))) : ((*(((Y_ERROR) >= 22 && (Y_ERROR) <= 29) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0X01) + 0x20)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 10 && (Y_ERROR) <= 13) || ((Y_ERROR) >= 50 && (Y_ERROR) <= 53)) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x04) + 0x20)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) >= 30 && (Y_ERROR) <= 37) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x07) + 0x20)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 18 && (Y_ERROR) <= 21) || (Y_ERROR) == 38) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0A) + 0x20)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 0 && (Y_ERROR) <= 3) || (Y_ERROR) == 5) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0D) + 0x20)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) >= 54 && (Y_ERROR) <= 61) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x10) + 0x20)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 39 && (Y_ERROR) <= 41) || (Y_ERROR) == 4) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x13) + 0x20)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 6 && (Y_ERROR) <= 9) || (Y_ERROR) == 16 || (Y_ERROR) == 17) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x101)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) == 14 || (Y_ERROR) == 15) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x104)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) >= 62 && (Y_ERROR) <= 69) ? &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x107)) 
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : &
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x10A))
# 162 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 ))))))))))) &= ~(1UL << ((((Y_ERROR) >= 7 && (Y_ERROR) <= 9) ? (Y_ERROR) - 3 : (((Y_ERROR) >= 10 && (Y_ERROR) <= 13) ? (Y_ERROR) - 6 : (((Y_ERROR) >= 22 && (Y_ERROR) <= 29) ? (Y_ERROR) - 22 : (((Y_ERROR) >= 30 && (Y_ERROR) <= 37) ? 37 - (Y_ERROR) : (((Y_ERROR) >= 39 && (Y_ERROR) <= 41) ? 41 - (Y_ERROR) : (((Y_ERROR) >= 42 && (Y_ERROR) <= 49) ? 49 - (Y_ERROR) : (((Y_ERROR) >= 50 && (Y_ERROR) <= 53) ? 53 - (Y_ERROR) : (((Y_ERROR) >= 54 && (Y_ERROR) <= 61) ? (Y_ERROR) - 54 : (((Y_ERROR) >= 62 && (Y_ERROR) <= 69) ? (Y_ERROR) - 62 : (((Y_ERROR) == 0 || (Y_ERROR) == 15 || (Y_ERROR) == 17 || (Y_ERROR) == 21) ? 0 : (((Y_ERROR) == 1 || (Y_ERROR) == 14 || (Y_ERROR) == 16 || (Y_ERROR) == 20) ? 1 : (((Y_ERROR) == 19) ? 2 : (((Y_ERROR) == 5 || (Y_ERROR) == 6 || (Y_ERROR) == 18) ? 3 : (((Y_ERROR) == 2) ? 4 : (((Y_ERROR) == 3 || (Y_ERROR) == 4) ? 5 : 7))))))))))))))))))); } } else { pinMode((Y_ERROR), (0x0)); };


  /* Self Check */
  for (size_t motor_index = 0; motor_index < MOTOR_COUNT; ++motor_index)
  {
    HardwareSerial & serial_stream = *(serial_stream_ptrs[motor_index]); // set serial stream
    TMC2209 & stepper_driver = stepper_drivers[motor_index]; // setup alias for  object in object array
    stepper_driver.setup(serial_stream); // get communications going
    if (stepper_driver.isSetupAndCommunicating()) // if communicating
    {
      Serial.print("TMC2209 # ");
      Serial.print(motor_index, 10);
      Serial.print(" is up and running \n");
    } else {
      Serial.print("TMC2209 # ");
      Serial.print(motor_index, 10);
      Serial.print(" is not communicating \n");
    }
  }

  stepper_controller.setup(CHIP_SELECT_PIN,CLOCK_FREQUENCY_MHZ); // setup TMC429
  stepper_controller.initialize(); // initialize TMC429
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
    stepper_driver.setRunCurrent(RUN_CURRENT); // run current setup
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
# 215 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
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
  query = 2; // stage finished moving, ready for coordinate input
}

void loop()
{
  if(halt) {
    stepper_controller.stopAll();
    while(1){};
  }
  X_actual = stepper_controller.getActualPosition(X_MOTOR);
  Y_actual = stepper_controller.getActualPosition(Y_MOTOR);
  X_target = stepper_controller.getTargetPosition(X_MOTOR);
  Y_target = stepper_controller.getTargetPosition(Y_MOTOR);

  // update controller status
  status = stepper_controller.getStatus();
  if (status.at_target_position_0 && status.at_target_position_1) {
    query = 2; // 0b10, stage finished moving
  } else {
    query = 1; // 0b01, stage moving
  }

  // move the motors
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
  }


}

void moveStageTo(int32_t x, int32_t y)
{
  stepper_controller.setTargetPosition(X_MOTOR, x); // set X target coordinate
  stepper_controller.setTargetPosition(Y_MOTOR, y); // set Y target coordinate
}

/* Homing procedure, assume TMC2209 StallGuard output as limit switch */
void homing()
{
  // first move the motors out of the axis minimums (if they are in)
  stepper_controller.setTargetPosition(X_MOTOR, MICROSTEPS_PER_REV); // move to the right by 1 rev
  stepper_controller.setTargetPosition(Y_MOTOR, -MICROSTEPS_PER_REV); // move to the top ^

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

    /* // 3. Stepper motor automatically stops upon stall. Move away by 1 full step to avoid origin stall

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

    } */
# 337 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
    // 4. Reset TMC429 reference point
    stepper_controller.setActualPosition(motor_index, 0);
    stepper_controller.setTargetPosition(motor_index, 0);
  }

  /* // ************ Continue homing procedure, acquire MAXIMUM POSITION *************



  // begin moving stage to absolute maximum

  stepper_controller.setTargetPosition(X_MOTOR, 0x7FFFFFFF);

  stepper_controller.setTargetPosition(Y_MOTOR, 0x80000000);



  uint8_t counter = 0;

  while(counter < 2)

  {

    if (digitalReadFast(X_ERROR) == 0) 

    {

      stepper_controller.stop(X_MOTOR); // stop X motor, which sets it into speed mode

      X_max = stepper_controller.getActualPosition(X_MOTOR) - 256; // acquire max coordinate

      stepper_controller.setSoftMode(X_MOTOR); // set soft deccelerate

      counter++;

    }



    if (digitalReadFast(Y_ERROR) == 0) 

    {

      stepper_controller.stop(Y_MOTOR); // stop Y motor 

      Y_max = stepper_controller.getActualPosition(Y_MOTOR) - 256; // acquire max coordinate

      stepper_controller.setSoftMode(Y_MOTOR); // set soft deccelerate

      counter++;

    } 

  } */
# 368 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
  // ************ Complete homing procedure, navigate to origin point. *************
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
  for(size_t i = 0; i <= 5000; ++i) {
    if(i == 5000) {
      sprintf(debugBuffer, "read()-> timeout, read failed"); // try 5000 times to read buffer
      dispBuffer();
      commandBuffer = 0; // if nothing was received break
      return;
    }
    if (Wire.available() == 0) continue;
    currRead = static_cast<uint8_t>(Wire.read()); // wait for scribble to appear
    if(currRead == 0x5a) {
      commandBuffer = static_cast<uint64_t>(0x5a) << (64 - 8 * 1); // reset command buffer
    }
  }
  // Wait for the buffer to receive at least 7 bytes
  while(Wire.available() < 7) {};
  // read bytes 2-8
  for(size_t i = 2; i < 8; ++i) { // wire.read returns one byte at a time from 32-byte hardware buffer
    currRead = static_cast<uint8_t>(Wire.read()); // return one byte
    commandBuffer &= ~(static_cast<uint64_t>(0b11111111) << (64 - 8 * i)); // Clear designated byte
    commandBuffer |= static_cast<uint64_t>(currRead) << (64 - 8 * i); // Set command buffer
    dispBuffer();
  }
  sprintf(debugBuffer, "read()-> read success");
  dispBuffer(); // display read success
}

void write() {
  if(commandBuffer == 0) {
    sprintf(debugBuffer, "write()-> command buffer is empty, write failed");
    dispBuffer();
  }
  commandBuffer &= ~(static_cast<uint64_t>(0b11111111) << (64 - 8 * 7)); // clear status byte
  commandBuffer |= static_cast<uint64_t>(query) << (64 - 8 * 7); // update status
  commandBuffer &= ~(static_cast<uint64_t>(0b11111111)); // clear Chksum
  commandBuffer |= static_cast<uint64_t>(getCheckSum()); // update checksum
  int ret = Wire.write(reinterpret_cast<uint8_t*>(&commandBuffer), 8);
  sprintf(debugBuffer, "write() -> bytes written: %d", ret);
  dispBuffer();
}

void execute() {
  size_t funcInd = *(reinterpret_cast<uint8_t*>(&commandBuffer) + 6);
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

void halt() {
  stageHalt = true;
}

void getWidth() {
  *reinterpret_cast<int32_t*>((reinterpret_cast<uint8_t*>(&commandBuffer) + 5)) = MICROSTEPS_PER_AXIS;
}

void getHeight() {
  *reinterpret_cast<int32_t*>((reinterpret_cast<uint8_t*>(&commandBuffer) + 5)) = MICROSTEPS_PER_AXIS;
}

void getX() {
  *reinterpret_cast<int32_t*>((reinterpret_cast<uint8_t*>(&commandBuffer) + 5)) = X_actual;
}

void getY() {
  *reinterpret_cast<int32_t*>((reinterpret_cast<uint8_t*>(&commandBuffer) + 5)) = Y_actual;
}

void setX() {
  X_target = *reinterpret_cast<int32_t*>((reinterpret_cast<uint8_t*>(&commandBuffer) + 5));
}

void setY() {
  Y_target = *reinterpret_cast<int32_t*>((reinterpret_cast<uint8_t*>(&commandBuffer) + 5));
}

void calib() {
  calibration = *reinterpret_cast<int16_t*>((reinterpret_cast<uint8_t*>(&commandBuffer) + 5)) > 0;
  calibrationComplete = *reinterpret_cast<int16_t*>((reinterpret_cast<uint8_t*>(&commandBuffer) + 5)) == 0;
}

uint8_t getCheckSum() {
  uint8_t chksum = 0xFF, sub = 0;
  for(size_t i= 1; i < 8; i++)
    sub += *(reinterpret_cast<uint8_t*>(&commandBuffer) + i);
  chksum = chksum - sub;
  sprintf(debugBuffer, "getCheckSum()-> check-sum: %d", chksum);
  dispBuffer();
  return chksum;
}
