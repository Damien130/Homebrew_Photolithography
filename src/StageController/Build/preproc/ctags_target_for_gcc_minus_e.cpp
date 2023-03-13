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
# 25 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 2


/* ATMega2560 Pin Mapping */
/* Digital Pins:

  RESET: RESET pin for emergency stop

  2: TMC2209_X error flag (PE4, 6) Interrupt 5 

  3: TMC2209_Y error flag (PE5, 7) Interrupt 6

  49: TMC429 Chip Select (PL0, 35)

*/
# 34 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
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

/* Global variables */
uint8_t test = 85;
uint8_t test1 = 170;
enum Flags
{ MOTOR_COUNT = 2,
  X_MOTOR = 1,
  Y_MOTOR = 2
};
const long SERIAL_BAUD_RATE = 115200;
volatile bool dataStrobe = false, addressStrobe = false; // Neubus ISR flags
int32_t X_target = 0, X_actual = 0, Y_target = 0, Y_actual = 0; // plane location, initialize to 0
int32_t X_max, Y_max; // maximum coordinates for each axis

/* TMC2209 driver settings */
/* Note: Current TMC2209 setup doesn't utilize UART address for simplicity of troubleshooting

   Instead, UART channel 1 and 2 is used for separate controllers 

   NOT gate required for output of TMC2209 error detection */
# 63 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
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
# 84 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
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
TMC429 stepper_controller; // TMC429 Stepper Controller

/* Instantiate NeuBus interface*/
NeuBus Bus;

void setup()
{
  Serial.begin(SERIAL_BAUD_RATE); // Serial console on serial0 @ 15200 baud

  /* ATMega2560 Setup */
  if (__builtin_constant_p(X_ERROR) && __builtin_constant_p(0x0)) { if (0x0 == 0x2) { ((0x0) ? ((*(((X_ERROR) >= 22 && (X_ERROR) <= 29) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0X01) + 0x20)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 10 && (X_ERROR) <= 13) || ((X_ERROR) >= 50 && (X_ERROR) <= 53)) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x04) + 0x20)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) >= 30 && (X_ERROR) <= 37) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x07) + 0x20)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 18 && (X_ERROR) <= 21) || (X_ERROR) == 38) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0A) + 0x20)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 0 && (X_ERROR) <= 3) || (X_ERROR) == 5) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0D) + 0x20)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) >= 54 && (X_ERROR) <= 61) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x10) + 0x20)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 39 && (X_ERROR) <= 41) || (X_ERROR) == 4) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x13) + 0x20)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 6 && (X_ERROR) <= 9) || (X_ERROR) == 16 || (X_ERROR) == 17) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x101)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) == 14 || (X_ERROR) == 15) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x104)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) >= 62 && (X_ERROR) <= 69) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x107)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x10A))
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 ))))))))))) |= (1UL << ((((X_ERROR) >= 7 && (X_ERROR) <= 9) ? (X_ERROR) - 3 : (((X_ERROR) >= 10 && (X_ERROR) <= 13) ? (X_ERROR) - 6 : (((X_ERROR) >= 22 && (X_ERROR) <= 29) ? (X_ERROR) - 22 : (((X_ERROR) >= 30 && (X_ERROR) <= 37) ? 37 - (X_ERROR) : (((X_ERROR) >= 39 && (X_ERROR) <= 41) ? 41 - (X_ERROR) : (((X_ERROR) >= 42 && (X_ERROR) <= 49) ? 49 - (X_ERROR) : (((X_ERROR) >= 50 && (X_ERROR) <= 53) ? 53 - (X_ERROR) : (((X_ERROR) >= 54 && (X_ERROR) <= 61) ? (X_ERROR) - 54 : (((X_ERROR) >= 62 && (X_ERROR) <= 69) ? (X_ERROR) - 62 : (((X_ERROR) == 0 || (X_ERROR) == 15 || (X_ERROR) == 17 || (X_ERROR) == 21) ? 0 : (((X_ERROR) == 1 || (X_ERROR) == 14 || (X_ERROR) == 16 || (X_ERROR) == 20) ? 1 : (((X_ERROR) == 19) ? 2 : (((X_ERROR) == 5 || (X_ERROR) == 6 || (X_ERROR) == 18) ? 3 : (((X_ERROR) == 2) ? 4 : (((X_ERROR) == 3 || (X_ERROR) == 4) ? 5 : 7)))))))))))))))))) : ((*(((X_ERROR) >= 22 && (X_ERROR) <= 29) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0X01) + 0x20)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 10 && (X_ERROR) <= 13) || ((X_ERROR) >= 50 && (X_ERROR) <= 53)) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x04) + 0x20)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) >= 30 && (X_ERROR) <= 37) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x07) + 0x20)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 18 && (X_ERROR) <= 21) || (X_ERROR) == 38) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0A) + 0x20)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 0 && (X_ERROR) <= 3) || (X_ERROR) == 5) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0D) + 0x20)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) >= 54 && (X_ERROR) <= 61) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x10) + 0x20)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 39 && (X_ERROR) <= 41) || (X_ERROR) == 4) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x13) + 0x20)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 6 && (X_ERROR) <= 9) || (X_ERROR) == 16 || (X_ERROR) == 17) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x101)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) == 14 || (X_ERROR) == 15) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x104)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) >= 62 && (X_ERROR) <= 69) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x107)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x10A))
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 ))))))))))) &= ~(1UL << ((((X_ERROR) >= 7 && (X_ERROR) <= 9) ? (X_ERROR) - 3 : (((X_ERROR) >= 10 && (X_ERROR) <= 13) ? (X_ERROR) - 6 : (((X_ERROR) >= 22 && (X_ERROR) <= 29) ? (X_ERROR) - 22 : (((X_ERROR) >= 30 && (X_ERROR) <= 37) ? 37 - (X_ERROR) : (((X_ERROR) >= 39 && (X_ERROR) <= 41) ? 41 - (X_ERROR) : (((X_ERROR) >= 42 && (X_ERROR) <= 49) ? 49 - (X_ERROR) : (((X_ERROR) >= 50 && (X_ERROR) <= 53) ? 53 - (X_ERROR) : (((X_ERROR) >= 54 && (X_ERROR) <= 61) ? (X_ERROR) - 54 : (((X_ERROR) >= 62 && (X_ERROR) <= 69) ? (X_ERROR) - 62 : (((X_ERROR) == 0 || (X_ERROR) == 15 || (X_ERROR) == 17 || (X_ERROR) == 21) ? 0 : (((X_ERROR) == 1 || (X_ERROR) == 14 || (X_ERROR) == 16 || (X_ERROR) == 20) ? 1 : (((X_ERROR) == 19) ? 2 : (((X_ERROR) == 5 || (X_ERROR) == 6 || (X_ERROR) == 18) ? 3 : (((X_ERROR) == 2) ? 4 : (((X_ERROR) == 3 || (X_ERROR) == 4) ? 5 : 7))))))))))))))))))); ((0x1) ? ((*(((X_ERROR) >= 22 && (X_ERROR) <= 29) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0X02) + 0x20)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 10 && (X_ERROR) <= 13) || ((X_ERROR) >= 50 && (X_ERROR) <= 53)) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x05) + 0x20)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) >= 30 && (X_ERROR) <= 37) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x08) + 0x20)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 18 && (X_ERROR) <= 21) || (X_ERROR) == 38) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0B) + 0x20)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 0 && (X_ERROR) <= 3) || (X_ERROR) == 5) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0E) + 0x20)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) >= 54 && (X_ERROR) <= 61) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x11) + 0x20)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 39 && (X_ERROR) <= 41) || (X_ERROR) == 4) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x14) + 0x20)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 6 && (X_ERROR) <= 9) || (X_ERROR) == 16 || (X_ERROR) == 17) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x102)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) == 14 || (X_ERROR) == 15) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x105)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) >= 62 && (X_ERROR) <= 69) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x108)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x10B))
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 ))))))))))) |= (1UL << ((((X_ERROR) >= 7 && (X_ERROR) <= 9) ? (X_ERROR) - 3 : (((X_ERROR) >= 10 && (X_ERROR) <= 13) ? (X_ERROR) - 6 : (((X_ERROR) >= 22 && (X_ERROR) <= 29) ? (X_ERROR) - 22 : (((X_ERROR) >= 30 && (X_ERROR) <= 37) ? 37 - (X_ERROR) : (((X_ERROR) >= 39 && (X_ERROR) <= 41) ? 41 - (X_ERROR) : (((X_ERROR) >= 42 && (X_ERROR) <= 49) ? 49 - (X_ERROR) : (((X_ERROR) >= 50 && (X_ERROR) <= 53) ? 53 - (X_ERROR) : (((X_ERROR) >= 54 && (X_ERROR) <= 61) ? (X_ERROR) - 54 : (((X_ERROR) >= 62 && (X_ERROR) <= 69) ? (X_ERROR) - 62 : (((X_ERROR) == 0 || (X_ERROR) == 15 || (X_ERROR) == 17 || (X_ERROR) == 21) ? 0 : (((X_ERROR) == 1 || (X_ERROR) == 14 || (X_ERROR) == 16 || (X_ERROR) == 20) ? 1 : (((X_ERROR) == 19) ? 2 : (((X_ERROR) == 5 || (X_ERROR) == 6 || (X_ERROR) == 18) ? 3 : (((X_ERROR) == 2) ? 4 : (((X_ERROR) == 3 || (X_ERROR) == 4) ? 5 : 7)))))))))))))))))) : ((*(((X_ERROR) >= 22 && (X_ERROR) <= 29) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0X02) + 0x20)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 10 && (X_ERROR) <= 13) || ((X_ERROR) >= 50 && (X_ERROR) <= 53)) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x05) + 0x20)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) >= 30 && (X_ERROR) <= 37) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x08) + 0x20)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 18 && (X_ERROR) <= 21) || (X_ERROR) == 38) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0B) + 0x20)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 0 && (X_ERROR) <= 3) || (X_ERROR) == 5) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0E) + 0x20)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) >= 54 && (X_ERROR) <= 61) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x11) + 0x20)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 39 && (X_ERROR) <= 41) || (X_ERROR) == 4) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x14) + 0x20)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 6 && (X_ERROR) <= 9) || (X_ERROR) == 16 || (X_ERROR) == 17) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x102)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) == 14 || (X_ERROR) == 15) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x105)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) >= 62 && (X_ERROR) <= 69) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x108)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x10B))
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 ))))))))))) &= ~(1UL << ((((X_ERROR) >= 7 && (X_ERROR) <= 9) ? (X_ERROR) - 3 : (((X_ERROR) >= 10 && (X_ERROR) <= 13) ? (X_ERROR) - 6 : (((X_ERROR) >= 22 && (X_ERROR) <= 29) ? (X_ERROR) - 22 : (((X_ERROR) >= 30 && (X_ERROR) <= 37) ? 37 - (X_ERROR) : (((X_ERROR) >= 39 && (X_ERROR) <= 41) ? 41 - (X_ERROR) : (((X_ERROR) >= 42 && (X_ERROR) <= 49) ? 49 - (X_ERROR) : (((X_ERROR) >= 50 && (X_ERROR) <= 53) ? 53 - (X_ERROR) : (((X_ERROR) >= 54 && (X_ERROR) <= 61) ? (X_ERROR) - 54 : (((X_ERROR) >= 62 && (X_ERROR) <= 69) ? (X_ERROR) - 62 : (((X_ERROR) == 0 || (X_ERROR) == 15 || (X_ERROR) == 17 || (X_ERROR) == 21) ? 0 : (((X_ERROR) == 1 || (X_ERROR) == 14 || (X_ERROR) == 16 || (X_ERROR) == 20) ? 1 : (((X_ERROR) == 19) ? 2 : (((X_ERROR) == 5 || (X_ERROR) == 6 || (X_ERROR) == 18) ? 3 : (((X_ERROR) == 2) ? 4 : (((X_ERROR) == 3 || (X_ERROR) == 4) ? 5 : 7))))))))))))))))))); } else { ((0x0) ? ((*(((X_ERROR) >= 22 && (X_ERROR) <= 29) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0X01) + 0x20)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 10 && (X_ERROR) <= 13) || ((X_ERROR) >= 50 && (X_ERROR) <= 53)) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x04) + 0x20)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) >= 30 && (X_ERROR) <= 37) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x07) + 0x20)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 18 && (X_ERROR) <= 21) || (X_ERROR) == 38) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0A) + 0x20)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 0 && (X_ERROR) <= 3) || (X_ERROR) == 5) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0D) + 0x20)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) >= 54 && (X_ERROR) <= 61) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x10) + 0x20)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 39 && (X_ERROR) <= 41) || (X_ERROR) == 4) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x13) + 0x20)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 6 && (X_ERROR) <= 9) || (X_ERROR) == 16 || (X_ERROR) == 17) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x101)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) == 14 || (X_ERROR) == 15) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x104)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) >= 62 && (X_ERROR) <= 69) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x107)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x10A))
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 ))))))))))) |= (1UL << ((((X_ERROR) >= 7 && (X_ERROR) <= 9) ? (X_ERROR) - 3 : (((X_ERROR) >= 10 && (X_ERROR) <= 13) ? (X_ERROR) - 6 : (((X_ERROR) >= 22 && (X_ERROR) <= 29) ? (X_ERROR) - 22 : (((X_ERROR) >= 30 && (X_ERROR) <= 37) ? 37 - (X_ERROR) : (((X_ERROR) >= 39 && (X_ERROR) <= 41) ? 41 - (X_ERROR) : (((X_ERROR) >= 42 && (X_ERROR) <= 49) ? 49 - (X_ERROR) : (((X_ERROR) >= 50 && (X_ERROR) <= 53) ? 53 - (X_ERROR) : (((X_ERROR) >= 54 && (X_ERROR) <= 61) ? (X_ERROR) - 54 : (((X_ERROR) >= 62 && (X_ERROR) <= 69) ? (X_ERROR) - 62 : (((X_ERROR) == 0 || (X_ERROR) == 15 || (X_ERROR) == 17 || (X_ERROR) == 21) ? 0 : (((X_ERROR) == 1 || (X_ERROR) == 14 || (X_ERROR) == 16 || (X_ERROR) == 20) ? 1 : (((X_ERROR) == 19) ? 2 : (((X_ERROR) == 5 || (X_ERROR) == 6 || (X_ERROR) == 18) ? 3 : (((X_ERROR) == 2) ? 4 : (((X_ERROR) == 3 || (X_ERROR) == 4) ? 5 : 7)))))))))))))))))) : ((*(((X_ERROR) >= 22 && (X_ERROR) <= 29) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0X01) + 0x20)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 10 && (X_ERROR) <= 13) || ((X_ERROR) >= 50 && (X_ERROR) <= 53)) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x04) + 0x20)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) >= 30 && (X_ERROR) <= 37) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x07) + 0x20)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 18 && (X_ERROR) <= 21) || (X_ERROR) == 38) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0A) + 0x20)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 0 && (X_ERROR) <= 3) || (X_ERROR) == 5) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0D) + 0x20)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) >= 54 && (X_ERROR) <= 61) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x10) + 0x20)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 39 && (X_ERROR) <= 41) || (X_ERROR) == 4) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x13) + 0x20)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((X_ERROR) >= 6 && (X_ERROR) <= 9) || (X_ERROR) == 16 || (X_ERROR) == 17) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x101)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) == 14 || (X_ERROR) == 15) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x104)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((X_ERROR) >= 62 && (X_ERROR) <= 69) ? &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x107)) 
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : &
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x10A))
# 106 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 ))))))))))) &= ~(1UL << ((((X_ERROR) >= 7 && (X_ERROR) <= 9) ? (X_ERROR) - 3 : (((X_ERROR) >= 10 && (X_ERROR) <= 13) ? (X_ERROR) - 6 : (((X_ERROR) >= 22 && (X_ERROR) <= 29) ? (X_ERROR) - 22 : (((X_ERROR) >= 30 && (X_ERROR) <= 37) ? 37 - (X_ERROR) : (((X_ERROR) >= 39 && (X_ERROR) <= 41) ? 41 - (X_ERROR) : (((X_ERROR) >= 42 && (X_ERROR) <= 49) ? 49 - (X_ERROR) : (((X_ERROR) >= 50 && (X_ERROR) <= 53) ? 53 - (X_ERROR) : (((X_ERROR) >= 54 && (X_ERROR) <= 61) ? (X_ERROR) - 54 : (((X_ERROR) >= 62 && (X_ERROR) <= 69) ? (X_ERROR) - 62 : (((X_ERROR) == 0 || (X_ERROR) == 15 || (X_ERROR) == 17 || (X_ERROR) == 21) ? 0 : (((X_ERROR) == 1 || (X_ERROR) == 14 || (X_ERROR) == 16 || (X_ERROR) == 20) ? 1 : (((X_ERROR) == 19) ? 2 : (((X_ERROR) == 5 || (X_ERROR) == 6 || (X_ERROR) == 18) ? 3 : (((X_ERROR) == 2) ? 4 : (((X_ERROR) == 3 || (X_ERROR) == 4) ? 5 : 7))))))))))))))))))); } } else { pinMode((X_ERROR), (0x0)); };
  if (__builtin_constant_p(Y_ERROR) && __builtin_constant_p(0x0)) { if (0x0 == 0x2) { ((0x0) ? ((*(((Y_ERROR) >= 22 && (Y_ERROR) <= 29) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0X01) + 0x20)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 10 && (Y_ERROR) <= 13) || ((Y_ERROR) >= 50 && (Y_ERROR) <= 53)) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x04) + 0x20)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) >= 30 && (Y_ERROR) <= 37) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x07) + 0x20)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 18 && (Y_ERROR) <= 21) || (Y_ERROR) == 38) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0A) + 0x20)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 0 && (Y_ERROR) <= 3) || (Y_ERROR) == 5) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0D) + 0x20)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) >= 54 && (Y_ERROR) <= 61) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x10) + 0x20)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 39 && (Y_ERROR) <= 41) || (Y_ERROR) == 4) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x13) + 0x20)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 6 && (Y_ERROR) <= 9) || (Y_ERROR) == 16 || (Y_ERROR) == 17) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x101)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) == 14 || (Y_ERROR) == 15) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x104)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) >= 62 && (Y_ERROR) <= 69) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x107)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x10A))
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 ))))))))))) |= (1UL << ((((Y_ERROR) >= 7 && (Y_ERROR) <= 9) ? (Y_ERROR) - 3 : (((Y_ERROR) >= 10 && (Y_ERROR) <= 13) ? (Y_ERROR) - 6 : (((Y_ERROR) >= 22 && (Y_ERROR) <= 29) ? (Y_ERROR) - 22 : (((Y_ERROR) >= 30 && (Y_ERROR) <= 37) ? 37 - (Y_ERROR) : (((Y_ERROR) >= 39 && (Y_ERROR) <= 41) ? 41 - (Y_ERROR) : (((Y_ERROR) >= 42 && (Y_ERROR) <= 49) ? 49 - (Y_ERROR) : (((Y_ERROR) >= 50 && (Y_ERROR) <= 53) ? 53 - (Y_ERROR) : (((Y_ERROR) >= 54 && (Y_ERROR) <= 61) ? (Y_ERROR) - 54 : (((Y_ERROR) >= 62 && (Y_ERROR) <= 69) ? (Y_ERROR) - 62 : (((Y_ERROR) == 0 || (Y_ERROR) == 15 || (Y_ERROR) == 17 || (Y_ERROR) == 21) ? 0 : (((Y_ERROR) == 1 || (Y_ERROR) == 14 || (Y_ERROR) == 16 || (Y_ERROR) == 20) ? 1 : (((Y_ERROR) == 19) ? 2 : (((Y_ERROR) == 5 || (Y_ERROR) == 6 || (Y_ERROR) == 18) ? 3 : (((Y_ERROR) == 2) ? 4 : (((Y_ERROR) == 3 || (Y_ERROR) == 4) ? 5 : 7)))))))))))))))))) : ((*(((Y_ERROR) >= 22 && (Y_ERROR) <= 29) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0X01) + 0x20)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 10 && (Y_ERROR) <= 13) || ((Y_ERROR) >= 50 && (Y_ERROR) <= 53)) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x04) + 0x20)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) >= 30 && (Y_ERROR) <= 37) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x07) + 0x20)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 18 && (Y_ERROR) <= 21) || (Y_ERROR) == 38) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0A) + 0x20)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 0 && (Y_ERROR) <= 3) || (Y_ERROR) == 5) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0D) + 0x20)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) >= 54 && (Y_ERROR) <= 61) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x10) + 0x20)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 39 && (Y_ERROR) <= 41) || (Y_ERROR) == 4) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x13) + 0x20)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 6 && (Y_ERROR) <= 9) || (Y_ERROR) == 16 || (Y_ERROR) == 17) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x101)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) == 14 || (Y_ERROR) == 15) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x104)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) >= 62 && (Y_ERROR) <= 69) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x107)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x10A))
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 ))))))))))) &= ~(1UL << ((((Y_ERROR) >= 7 && (Y_ERROR) <= 9) ? (Y_ERROR) - 3 : (((Y_ERROR) >= 10 && (Y_ERROR) <= 13) ? (Y_ERROR) - 6 : (((Y_ERROR) >= 22 && (Y_ERROR) <= 29) ? (Y_ERROR) - 22 : (((Y_ERROR) >= 30 && (Y_ERROR) <= 37) ? 37 - (Y_ERROR) : (((Y_ERROR) >= 39 && (Y_ERROR) <= 41) ? 41 - (Y_ERROR) : (((Y_ERROR) >= 42 && (Y_ERROR) <= 49) ? 49 - (Y_ERROR) : (((Y_ERROR) >= 50 && (Y_ERROR) <= 53) ? 53 - (Y_ERROR) : (((Y_ERROR) >= 54 && (Y_ERROR) <= 61) ? (Y_ERROR) - 54 : (((Y_ERROR) >= 62 && (Y_ERROR) <= 69) ? (Y_ERROR) - 62 : (((Y_ERROR) == 0 || (Y_ERROR) == 15 || (Y_ERROR) == 17 || (Y_ERROR) == 21) ? 0 : (((Y_ERROR) == 1 || (Y_ERROR) == 14 || (Y_ERROR) == 16 || (Y_ERROR) == 20) ? 1 : (((Y_ERROR) == 19) ? 2 : (((Y_ERROR) == 5 || (Y_ERROR) == 6 || (Y_ERROR) == 18) ? 3 : (((Y_ERROR) == 2) ? 4 : (((Y_ERROR) == 3 || (Y_ERROR) == 4) ? 5 : 7))))))))))))))))))); ((0x1) ? ((*(((Y_ERROR) >= 22 && (Y_ERROR) <= 29) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0X02) + 0x20)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 10 && (Y_ERROR) <= 13) || ((Y_ERROR) >= 50 && (Y_ERROR) <= 53)) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x05) + 0x20)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) >= 30 && (Y_ERROR) <= 37) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x08) + 0x20)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 18 && (Y_ERROR) <= 21) || (Y_ERROR) == 38) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0B) + 0x20)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 0 && (Y_ERROR) <= 3) || (Y_ERROR) == 5) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0E) + 0x20)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) >= 54 && (Y_ERROR) <= 61) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x11) + 0x20)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 39 && (Y_ERROR) <= 41) || (Y_ERROR) == 4) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x14) + 0x20)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 6 && (Y_ERROR) <= 9) || (Y_ERROR) == 16 || (Y_ERROR) == 17) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x102)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) == 14 || (Y_ERROR) == 15) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x105)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) >= 62 && (Y_ERROR) <= 69) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x108)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x10B))
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 ))))))))))) |= (1UL << ((((Y_ERROR) >= 7 && (Y_ERROR) <= 9) ? (Y_ERROR) - 3 : (((Y_ERROR) >= 10 && (Y_ERROR) <= 13) ? (Y_ERROR) - 6 : (((Y_ERROR) >= 22 && (Y_ERROR) <= 29) ? (Y_ERROR) - 22 : (((Y_ERROR) >= 30 && (Y_ERROR) <= 37) ? 37 - (Y_ERROR) : (((Y_ERROR) >= 39 && (Y_ERROR) <= 41) ? 41 - (Y_ERROR) : (((Y_ERROR) >= 42 && (Y_ERROR) <= 49) ? 49 - (Y_ERROR) : (((Y_ERROR) >= 50 && (Y_ERROR) <= 53) ? 53 - (Y_ERROR) : (((Y_ERROR) >= 54 && (Y_ERROR) <= 61) ? (Y_ERROR) - 54 : (((Y_ERROR) >= 62 && (Y_ERROR) <= 69) ? (Y_ERROR) - 62 : (((Y_ERROR) == 0 || (Y_ERROR) == 15 || (Y_ERROR) == 17 || (Y_ERROR) == 21) ? 0 : (((Y_ERROR) == 1 || (Y_ERROR) == 14 || (Y_ERROR) == 16 || (Y_ERROR) == 20) ? 1 : (((Y_ERROR) == 19) ? 2 : (((Y_ERROR) == 5 || (Y_ERROR) == 6 || (Y_ERROR) == 18) ? 3 : (((Y_ERROR) == 2) ? 4 : (((Y_ERROR) == 3 || (Y_ERROR) == 4) ? 5 : 7)))))))))))))))))) : ((*(((Y_ERROR) >= 22 && (Y_ERROR) <= 29) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0X02) + 0x20)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 10 && (Y_ERROR) <= 13) || ((Y_ERROR) >= 50 && (Y_ERROR) <= 53)) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x05) + 0x20)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) >= 30 && (Y_ERROR) <= 37) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x08) + 0x20)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 18 && (Y_ERROR) <= 21) || (Y_ERROR) == 38) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0B) + 0x20)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 0 && (Y_ERROR) <= 3) || (Y_ERROR) == 5) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0E) + 0x20)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) >= 54 && (Y_ERROR) <= 61) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x11) + 0x20)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 39 && (Y_ERROR) <= 41) || (Y_ERROR) == 4) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x14) + 0x20)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 6 && (Y_ERROR) <= 9) || (Y_ERROR) == 16 || (Y_ERROR) == 17) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x102)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) == 14 || (Y_ERROR) == 15) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x105)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) >= 62 && (Y_ERROR) <= 69) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x108)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x10B))
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 ))))))))))) &= ~(1UL << ((((Y_ERROR) >= 7 && (Y_ERROR) <= 9) ? (Y_ERROR) - 3 : (((Y_ERROR) >= 10 && (Y_ERROR) <= 13) ? (Y_ERROR) - 6 : (((Y_ERROR) >= 22 && (Y_ERROR) <= 29) ? (Y_ERROR) - 22 : (((Y_ERROR) >= 30 && (Y_ERROR) <= 37) ? 37 - (Y_ERROR) : (((Y_ERROR) >= 39 && (Y_ERROR) <= 41) ? 41 - (Y_ERROR) : (((Y_ERROR) >= 42 && (Y_ERROR) <= 49) ? 49 - (Y_ERROR) : (((Y_ERROR) >= 50 && (Y_ERROR) <= 53) ? 53 - (Y_ERROR) : (((Y_ERROR) >= 54 && (Y_ERROR) <= 61) ? (Y_ERROR) - 54 : (((Y_ERROR) >= 62 && (Y_ERROR) <= 69) ? (Y_ERROR) - 62 : (((Y_ERROR) == 0 || (Y_ERROR) == 15 || (Y_ERROR) == 17 || (Y_ERROR) == 21) ? 0 : (((Y_ERROR) == 1 || (Y_ERROR) == 14 || (Y_ERROR) == 16 || (Y_ERROR) == 20) ? 1 : (((Y_ERROR) == 19) ? 2 : (((Y_ERROR) == 5 || (Y_ERROR) == 6 || (Y_ERROR) == 18) ? 3 : (((Y_ERROR) == 2) ? 4 : (((Y_ERROR) == 3 || (Y_ERROR) == 4) ? 5 : 7))))))))))))))))))); } else { ((0x0) ? ((*(((Y_ERROR) >= 22 && (Y_ERROR) <= 29) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0X01) + 0x20)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 10 && (Y_ERROR) <= 13) || ((Y_ERROR) >= 50 && (Y_ERROR) <= 53)) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x04) + 0x20)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) >= 30 && (Y_ERROR) <= 37) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x07) + 0x20)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 18 && (Y_ERROR) <= 21) || (Y_ERROR) == 38) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0A) + 0x20)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 0 && (Y_ERROR) <= 3) || (Y_ERROR) == 5) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0D) + 0x20)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) >= 54 && (Y_ERROR) <= 61) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x10) + 0x20)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 39 && (Y_ERROR) <= 41) || (Y_ERROR) == 4) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x13) + 0x20)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 6 && (Y_ERROR) <= 9) || (Y_ERROR) == 16 || (Y_ERROR) == 17) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x101)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) == 14 || (Y_ERROR) == 15) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x104)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) >= 62 && (Y_ERROR) <= 69) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x107)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x10A))
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 ))))))))))) |= (1UL << ((((Y_ERROR) >= 7 && (Y_ERROR) <= 9) ? (Y_ERROR) - 3 : (((Y_ERROR) >= 10 && (Y_ERROR) <= 13) ? (Y_ERROR) - 6 : (((Y_ERROR) >= 22 && (Y_ERROR) <= 29) ? (Y_ERROR) - 22 : (((Y_ERROR) >= 30 && (Y_ERROR) <= 37) ? 37 - (Y_ERROR) : (((Y_ERROR) >= 39 && (Y_ERROR) <= 41) ? 41 - (Y_ERROR) : (((Y_ERROR) >= 42 && (Y_ERROR) <= 49) ? 49 - (Y_ERROR) : (((Y_ERROR) >= 50 && (Y_ERROR) <= 53) ? 53 - (Y_ERROR) : (((Y_ERROR) >= 54 && (Y_ERROR) <= 61) ? (Y_ERROR) - 54 : (((Y_ERROR) >= 62 && (Y_ERROR) <= 69) ? (Y_ERROR) - 62 : (((Y_ERROR) == 0 || (Y_ERROR) == 15 || (Y_ERROR) == 17 || (Y_ERROR) == 21) ? 0 : (((Y_ERROR) == 1 || (Y_ERROR) == 14 || (Y_ERROR) == 16 || (Y_ERROR) == 20) ? 1 : (((Y_ERROR) == 19) ? 2 : (((Y_ERROR) == 5 || (Y_ERROR) == 6 || (Y_ERROR) == 18) ? 3 : (((Y_ERROR) == 2) ? 4 : (((Y_ERROR) == 3 || (Y_ERROR) == 4) ? 5 : 7)))))))))))))))))) : ((*(((Y_ERROR) >= 22 && (Y_ERROR) <= 29) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0X01) + 0x20)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 10 && (Y_ERROR) <= 13) || ((Y_ERROR) >= 50 && (Y_ERROR) <= 53)) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x04) + 0x20)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) >= 30 && (Y_ERROR) <= 37) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x07) + 0x20)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 18 && (Y_ERROR) <= 21) || (Y_ERROR) == 38) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0A) + 0x20)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 0 && (Y_ERROR) <= 3) || (Y_ERROR) == 5) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0D) + 0x20)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) >= 54 && (Y_ERROR) <= 61) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x10) + 0x20)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 39 && (Y_ERROR) <= 41) || (Y_ERROR) == 4) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x13) + 0x20)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : ((((Y_ERROR) >= 6 && (Y_ERROR) <= 9) || (Y_ERROR) == 16 || (Y_ERROR) == 17) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x101)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) == 14 || (Y_ERROR) == 15) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x104)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : (((Y_ERROR) >= 62 && (Y_ERROR) <= 69) ? &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x107)) 
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 : &
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)(0x10A))
# 107 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
 ))))))))))) &= ~(1UL << ((((Y_ERROR) >= 7 && (Y_ERROR) <= 9) ? (Y_ERROR) - 3 : (((Y_ERROR) >= 10 && (Y_ERROR) <= 13) ? (Y_ERROR) - 6 : (((Y_ERROR) >= 22 && (Y_ERROR) <= 29) ? (Y_ERROR) - 22 : (((Y_ERROR) >= 30 && (Y_ERROR) <= 37) ? 37 - (Y_ERROR) : (((Y_ERROR) >= 39 && (Y_ERROR) <= 41) ? 41 - (Y_ERROR) : (((Y_ERROR) >= 42 && (Y_ERROR) <= 49) ? 49 - (Y_ERROR) : (((Y_ERROR) >= 50 && (Y_ERROR) <= 53) ? 53 - (Y_ERROR) : (((Y_ERROR) >= 54 && (Y_ERROR) <= 61) ? (Y_ERROR) - 54 : (((Y_ERROR) >= 62 && (Y_ERROR) <= 69) ? (Y_ERROR) - 62 : (((Y_ERROR) == 0 || (Y_ERROR) == 15 || (Y_ERROR) == 17 || (Y_ERROR) == 21) ? 0 : (((Y_ERROR) == 1 || (Y_ERROR) == 14 || (Y_ERROR) == 16 || (Y_ERROR) == 20) ? 1 : (((Y_ERROR) == 19) ? 2 : (((Y_ERROR) == 5 || (Y_ERROR) == 6 || (Y_ERROR) == 18) ? 3 : (((Y_ERROR) == 2) ? 4 : (((Y_ERROR) == 3 || (Y_ERROR) == 4) ? 5 : 7))))))))))))))))))); } } else { pinMode((Y_ERROR), (0x0)); };
  
# 108 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
 (*(volatile uint8_t *)((0x0A) + 0x20)) 
# 108 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
      = 
# 108 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
        (*(volatile uint8_t *)((0x0A) + 0x20)) 
# 108 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
             | 15; // PORTD 0 - 3 configure to input
  attachInterrupt(((nDATASTRB) == 2 ? 0 : ((nDATASTRB) == 3 ? 1 : ((nDATASTRB) >= 18 && (nDATASTRB) <= 21 ? 23 - (nDATASTRB) : -1))), onDATASTROBE, 2);
  attachInterrupt(((nADDRSTRB) == 2 ? 0 : ((nADDRSTRB) == 3 ? 1 : ((nADDRSTRB) >= 18 && (nADDRSTRB) <= 21 ? 23 - (nADDRSTRB) : -1))), onADDRSTROBE, 2);


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
# 163 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
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
  delay(1);
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

  // begin moving stage to absolute maximum
  stepper_controller.setTargetPosition(X_MOTOR, 0x7FFFFFFF);
  stepper_controller.setTargetPosition(Y_MOTOR, 0x80000000);

  uint8_t counter = 0;
  while(counter < 2)
  {
    if (( (int) (__builtin_constant_p((X_ERROR)) ) ? ( ( ((*((((X_ERROR)) >= 22 && ((X_ERROR)) <= 29) ? &
# 252 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
       (*(volatile uint8_t *)((0X00) + 0x20)) 
# 252 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
       : (((((X_ERROR)) >= 10 && ((X_ERROR)) <= 13) || (((X_ERROR)) >= 50 && ((X_ERROR)) <= 53)) ? &
# 252 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
       (*(volatile uint8_t *)((0X03) + 0x20)) 
# 252 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
       : ((((X_ERROR)) >= 30 && ((X_ERROR)) <= 37) ? &
# 252 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
       (*(volatile uint8_t *)((0x06) + 0x20)) 
# 252 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
       : (((((X_ERROR)) >= 18 && ((X_ERROR)) <= 21) || ((X_ERROR)) == 38) ? &
# 252 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
       (*(volatile uint8_t *)((0x09) + 0x20)) 
# 252 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
       : (((((X_ERROR)) >= 0 && ((X_ERROR)) <= 3) || ((X_ERROR)) == 5) ? &
# 252 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
       (*(volatile uint8_t *)((0x0C) + 0x20)) 
# 252 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
       : ((((X_ERROR)) >= 54 && ((X_ERROR)) <= 61) ? &
# 252 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
       (*(volatile uint8_t *)((0x0F) + 0x20)) 
# 252 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
       : (((((X_ERROR)) >= 39 && ((X_ERROR)) <= 41) || ((X_ERROR)) == 4) ? &
# 252 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
       (*(volatile uint8_t *)((0x12) + 0x20)) 
# 252 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
       : (((((X_ERROR)) >= 6 && ((X_ERROR)) <= 9) || ((X_ERROR)) == 16 || ((X_ERROR)) == 17) ? &
# 252 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
       (*(volatile uint8_t *)(0x100)) 
# 252 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
       : ((((X_ERROR)) == 14 || ((X_ERROR)) == 15) ? &
# 252 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
       (*(volatile uint8_t *)(0x103)) 
# 252 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
       : ((((X_ERROR)) >= 62 && ((X_ERROR)) <= 69) ? &
# 252 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
       (*(volatile uint8_t *)(0x106)) 
# 252 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
       : &
# 252 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
       (*(volatile uint8_t *)(0x109))
# 252 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
       ))))))))))) & (1UL << (((((X_ERROR)) >= 7 && ((X_ERROR)) <= 9) ? ((X_ERROR)) - 3 : ((((X_ERROR)) >= 10 && ((X_ERROR)) <= 13) ? ((X_ERROR)) - 6 : ((((X_ERROR)) >= 22 && ((X_ERROR)) <= 29) ? ((X_ERROR)) - 22 : ((((X_ERROR)) >= 30 && ((X_ERROR)) <= 37) ? 37 - ((X_ERROR)) : ((((X_ERROR)) >= 39 && ((X_ERROR)) <= 41) ? 41 - ((X_ERROR)) : ((((X_ERROR)) >= 42 && ((X_ERROR)) <= 49) ? 49 - ((X_ERROR)) : ((((X_ERROR)) >= 50 && ((X_ERROR)) <= 53) ? 53 - ((X_ERROR)) : ((((X_ERROR)) >= 54 && ((X_ERROR)) <= 61) ? ((X_ERROR)) - 54 : ((((X_ERROR)) >= 62 && ((X_ERROR)) <= 69) ? ((X_ERROR)) - 62 : ((((X_ERROR)) == 0 || ((X_ERROR)) == 15 || ((X_ERROR)) == 17 || ((X_ERROR)) == 21) ? 0 : ((((X_ERROR)) == 1 || ((X_ERROR)) == 14 || ((X_ERROR)) == 16 || ((X_ERROR)) == 20) ? 1 : ((((X_ERROR)) == 19) ? 2 : ((((X_ERROR)) == 5 || ((X_ERROR)) == 6 || ((X_ERROR)) == 18) ? 3 : ((((X_ERROR)) == 2) ? 4 : ((((X_ERROR)) == 3 || ((X_ERROR)) == 4) ? 5 : 7))))))))))))))))))) ? 0x1:0x0 ) : digitalRead(((X_ERROR))) ) == 0)
    {
      stepper_controller.stop(X_MOTOR); // stop X motor, which sets it into speed mode
      X_max = stepper_controller.getActualPosition(X_MOTOR) - 256; // acquire max coordinate
      stepper_controller.setSoftMode(X_MOTOR); // set soft deccelerate
      counter++;
    }

    if (( (int) (__builtin_constant_p((Y_ERROR)) ) ? ( ( ((*((((Y_ERROR)) >= 22 && ((Y_ERROR)) <= 29) ? &
# 260 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
       (*(volatile uint8_t *)((0X00) + 0x20)) 
# 260 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
       : (((((Y_ERROR)) >= 10 && ((Y_ERROR)) <= 13) || (((Y_ERROR)) >= 50 && ((Y_ERROR)) <= 53)) ? &
# 260 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
       (*(volatile uint8_t *)((0X03) + 0x20)) 
# 260 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
       : ((((Y_ERROR)) >= 30 && ((Y_ERROR)) <= 37) ? &
# 260 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
       (*(volatile uint8_t *)((0x06) + 0x20)) 
# 260 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
       : (((((Y_ERROR)) >= 18 && ((Y_ERROR)) <= 21) || ((Y_ERROR)) == 38) ? &
# 260 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
       (*(volatile uint8_t *)((0x09) + 0x20)) 
# 260 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
       : (((((Y_ERROR)) >= 0 && ((Y_ERROR)) <= 3) || ((Y_ERROR)) == 5) ? &
# 260 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
       (*(volatile uint8_t *)((0x0C) + 0x20)) 
# 260 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
       : ((((Y_ERROR)) >= 54 && ((Y_ERROR)) <= 61) ? &
# 260 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
       (*(volatile uint8_t *)((0x0F) + 0x20)) 
# 260 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
       : (((((Y_ERROR)) >= 39 && ((Y_ERROR)) <= 41) || ((Y_ERROR)) == 4) ? &
# 260 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
       (*(volatile uint8_t *)((0x12) + 0x20)) 
# 260 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
       : (((((Y_ERROR)) >= 6 && ((Y_ERROR)) <= 9) || ((Y_ERROR)) == 16 || ((Y_ERROR)) == 17) ? &
# 260 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
       (*(volatile uint8_t *)(0x100)) 
# 260 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
       : ((((Y_ERROR)) == 14 || ((Y_ERROR)) == 15) ? &
# 260 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
       (*(volatile uint8_t *)(0x103)) 
# 260 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
       : ((((Y_ERROR)) >= 62 && ((Y_ERROR)) <= 69) ? &
# 260 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
       (*(volatile uint8_t *)(0x106)) 
# 260 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
       : &
# 260 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino" 3
       (*(volatile uint8_t *)(0x109))
# 260 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\StageController.ino"
       ))))))))))) & (1UL << (((((Y_ERROR)) >= 7 && ((Y_ERROR)) <= 9) ? ((Y_ERROR)) - 3 : ((((Y_ERROR)) >= 10 && ((Y_ERROR)) <= 13) ? ((Y_ERROR)) - 6 : ((((Y_ERROR)) >= 22 && ((Y_ERROR)) <= 29) ? ((Y_ERROR)) - 22 : ((((Y_ERROR)) >= 30 && ((Y_ERROR)) <= 37) ? 37 - ((Y_ERROR)) : ((((Y_ERROR)) >= 39 && ((Y_ERROR)) <= 41) ? 41 - ((Y_ERROR)) : ((((Y_ERROR)) >= 42 && ((Y_ERROR)) <= 49) ? 49 - ((Y_ERROR)) : ((((Y_ERROR)) >= 50 && ((Y_ERROR)) <= 53) ? 53 - ((Y_ERROR)) : ((((Y_ERROR)) >= 54 && ((Y_ERROR)) <= 61) ? ((Y_ERROR)) - 54 : ((((Y_ERROR)) >= 62 && ((Y_ERROR)) <= 69) ? ((Y_ERROR)) - 62 : ((((Y_ERROR)) == 0 || ((Y_ERROR)) == 15 || ((Y_ERROR)) == 17 || ((Y_ERROR)) == 21) ? 0 : ((((Y_ERROR)) == 1 || ((Y_ERROR)) == 14 || ((Y_ERROR)) == 16 || ((Y_ERROR)) == 20) ? 1 : ((((Y_ERROR)) == 19) ? 2 : ((((Y_ERROR)) == 5 || ((Y_ERROR)) == 6 || ((Y_ERROR)) == 18) ? 3 : ((((Y_ERROR)) == 2) ? 4 : ((((Y_ERROR)) == 3 || ((Y_ERROR)) == 4) ? 5 : 7))))))))))))))))))) ? 0x1:0x0 ) : digitalRead(((Y_ERROR))) ) == 0)
    {
      stepper_controller.stop(Y_MOTOR); // stop Y motor 
      Y_max = stepper_controller.getActualPosition(Y_MOTOR) - 256; // acquire max coordinate
      stepper_controller.setSoftMode(Y_MOTOR); // set soft deccelerate
      counter++;
    }
  }

  // ************ Complete homing procedure, go to origin point. *************
  stepper_controller.setTargetPosition(X_MOTOR, 0);
  stepper_controller.setTargetPosition(Y_MOTOR, 0);
}


/* Interrupt handler for NeuBus architecture */
void onDATASTROBE()
{
  dataStrobe = true;
}

void onADDRSTROBE()
{
  addressStrobe = true;
}
