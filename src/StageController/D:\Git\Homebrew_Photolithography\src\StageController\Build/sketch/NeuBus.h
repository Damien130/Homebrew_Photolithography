#line 1 "/Users/damienhu/Documents/Git/Homebrew_Photolithography/src/StageController/NeuBus.h"
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

class NeuBus
{
    public:
        NeuBus(); // default constructor
         NeuBus(int8_t, int8_t, int8_t, int8_t, int8_t, int8_t, int8_t, int8_t, volatile uint8_t &, volatile uint8_t &);
        void deviceReady();
        void deviceNotReady();
        void readyToExpose();
        void notReadyToExpose();
        void sendData(uint8_t packet, volatile bool &DataStrobe);
        void sendAddress(uint8_t packet, volatile bool &AddressStrobe);
        uint8_t readData(volatile bool &DataStrobe);
        uint8_t readAddress(volatile bool &AddressStrobe);

    private:
        void dataReady();

};

#endif