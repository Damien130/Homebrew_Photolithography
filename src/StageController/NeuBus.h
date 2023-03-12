/*************************************************************
 * NeuBus driver library
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
#ifndef NEUBUS_H
#define NEUBUS_H
#include <Arduino.h>
#include <digitalWriteFast.h>
#include <RingBuf.h>

/****** Introducing the NeuBus ******
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
class NeuBus
{
    public:
        NeuBus(); // default constructor uses PORD D as Control, A as Address and C as Data
        // NeuBus(size_t ctlBusID, size_t addrBusID, size_t dataBusID);
        inline void deviceNotReady();
        inline void deviceReady();
        inline void inPosition(bool inPos);
        inline void readyToExpose(bool rdyExp);
        void send(uint8_t packet);
        inline void send(uint16_t packet, volatile bool& ALE);
        inline void send(uint32_t packet, volatile bool& ALE);
        inline uint8_t read();

    private:
        uint8_t addressLatched;
        RingBuf<uint8_t, 2> rcvBuff;
        RingBuf<uint32_t, 2> sndBuff;
        inline void dataReady();
        inline void send16BitMode(uint8_t);

};

#endif