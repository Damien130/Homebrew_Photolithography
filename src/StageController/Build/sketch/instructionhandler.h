#line 1 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\instructionhandler.h"
/*******************************************************************
 * Photolithography Stepper Stage Controller I2C Instruction Handler
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
**********************************************************************/


#ifndef INSTRUCTIONHANDLER_H
#define INSTRUCTIONHANDLER_H

#ifndef SCRIBBLE
#define SCRIBBLE 0x5a
#endif

#include <stdint.h>
#include <stdio.h>
class InstructionHandler {
  private:

	void halt();//0
	void getWidth(); //1
	void getHeight();//2
	void getX();//3
	void getY();//4
	void setX();//5
	void setY();//6
	void calib();//7


	// instruction handler function hash map
	void(InstructionHandler::*const functionMap[9])(void) = {
		&InstructionHandler::halt
		, &InstructionHandler::getWidth
		, &InstructionHandler::getHeight
		, &InstructionHandler::getX
		, &InstructionHandler::getY
		, &InstructionHandler::setX
		, &InstructionHandler::setY
		, &InstructionHandler::calib
	};

	char debugBuffer[50]; // serial console buffer
	uint64_t commandBuffer; // outgoing packet buffer
	bool* mutexLock; // muti threading lock
	int32_t* actualX, *actualY; 
	int32_t* targetX, *targetY;//commanded
	int32_t* width, *height;
	int8_t* status;
	bool* haltStatus;
	bool* calibStatus;
	bool* calibCompleteStatus;

	//wire handler
	int(*readFunction)(void);
	size_t(*writeFunction)(const uint8_t*, size_t);
	int(*availableFunction)(void);
	void(*debug)(char*);

	void dispBuffer();

	//event handler
	uint8_t getCheckSum();

  public:
	void execute();
	void read();
	void write();
	bool debugMode = true;

	InstructionHandler(
		int32_t *actualX, int32_t* actualY
		, int32_t* targetX, int32_t* targetY
		, int32_t *width, int32_t* height
		, int8_t *status
		, int(*readFunction)(void)
		, size_t(*writeFunction)(const uint8_t*, size_t)
		, int(*availableFunction)(void)
		, void(*debug)(char*)
	);

};

inline void InstructionHandler::read() {
	uint8_t currRead = 0;
	//check 1st byte and assert it to be scribble
	for(size_t i = 0; i <= 5000; ++i) {
		if(i == 5000) {
			sprintf(debugBuffer, "read()-> timeout, read failed");
			dispBuffer();
			commandBuffer = 0;
			return;
		}
		if(this->availableFunction() == 0) continue;
		currRead = static_cast<uint8_t>(this->readFunction());
		if(currRead == SCRIBBLE) {
			commandBuffer = static_cast<uint64_t>(SCRIBBLE) << (64 - 8 * 1);
		}
	}
	//wait for the buffer for at least seven bytes to read
	while(this->availableFunction() < 7) {};
	//read 2-8 bytes and assert 6th bit to be zero
	for(size_t i = 2; i <= 8; ++i) {
		currRead = static_cast<uint8_t>(this->readFunction());
		if(i == 7 ) {
			*status = currRead;
		} else {
			commandBuffer &= ~(static_cast<uint64_t>(0b11111111) << (64 - 8 * i));//CLEAR BITS
			commandBuffer |= static_cast<uint64_t>(currRead) << (64 - 8 * i); //SET BITS
			dispBuffer();
		}
	}
	sprintf(debugBuffer, "read()-> read success");
	dispBuffer();
}

inline void InstructionHandler::write() {
	if(commandBuffer == 0) {
		sprintf(debugBuffer, "write()-> command buffer is empty write failed");
		dispBuffer();
	}
	commandBuffer &= ~(static_cast<uint64_t>(0b11111111) << (64 - 8 * 7));
	commandBuffer |= static_cast<uint64_t>(*status) << (64 - 8 * 7);
	int ret = this->writeFunction(reinterpret_cast<uint8_t*>(&commandBuffer), 8);
	sprintf(debugBuffer, "write()-> bytes written: %d", ret);
	dispBuffer();
}

inline void InstructionHandler::halt() {
	*haltStatus = *reinterpret_cast<int32_t*>((reinterpret_cast<uint8_t*>(&commandBuffer) + 1)) > 0;
}

inline void InstructionHandler::getWidth() {
	*reinterpret_cast<int32_t*>((reinterpret_cast<uint8_t*>(&commandBuffer) + 1)) = *width;
}

inline void InstructionHandler::getHeight() {
	*reinterpret_cast<int32_t*>((reinterpret_cast<uint8_t*>(&commandBuffer) + 1)) = *height;
}

inline void InstructionHandler::getX() {
	*reinterpret_cast<int32_t*>((reinterpret_cast<uint8_t*>(&commandBuffer) + 1)) = *actualX;
}

inline void InstructionHandler::getY() {
	*reinterpret_cast<int32_t*>((reinterpret_cast<uint8_t*>(&commandBuffer) + 1)) = *actualY;
}

inline void InstructionHandler::setX() {
	*targetX = *reinterpret_cast<int32_t*>((reinterpret_cast<uint8_t*>(&commandBuffer) + 1));
}

inline void InstructionHandler::setY() {
	*targetY = *reinterpret_cast<int32_t*>((reinterpret_cast<uint8_t*>(&commandBuffer) + 1));
}

inline void InstructionHandler::calib() {
	*calibStatus = *reinterpret_cast<int16_t*>((reinterpret_cast<uint8_t*>(&commandBuffer) + 1)) > 0;
	*calibCompleteStatus = *reinterpret_cast<int16_t*>((reinterpret_cast<uint8_t*>(&commandBuffer) + 3)) > 0;
}

inline void InstructionHandler::dispBuffer() {
	if(debugMode) this->debug(this->debugBuffer);
}

inline uint8_t InstructionHandler::getCheckSum() {
	//TO-DO!
	uint8_t ret = 0;
	sprintf(debugBuffer, "getCheckSum()-> check-sum: %d", ret);
	dispBuffer();
	return ret;
}

inline void InstructionHandler::execute() {
	size_t funcInd = *(reinterpret_cast<uint8_t*>(&commandBuffer) + 1);
	if(funcInd < 0 || funcInd >= 9) {
		sprintf(debugBuffer, "execute()-> invalid command number: %d", static_cast<int>(funcInd));
		dispBuffer();
	}
	void (InstructionHandler::*func)(void) = *(functionMap + funcInd);
	(this->*func)();
}


inline InstructionHandler::InstructionHandler(
	int32_t *actualX, int32_t* actualY
	, int32_t* targetX, int32_t* targetY
	, int32_t *width, int32_t* height
	, int8_t *status
	, int(*readFunction)(void)
	, size_t(*writeFunction)(const uint8_t*, size_t)
	, int(*availableFunction)(void)
	, void(*debug)(char*)) {
	this->actualX = actualX;
	this->actualY = actualY;
	this->targetX = targetX;
	this->targetY = targetY;
	this->width = width;
	this->height = height;
	this->status = status;
	this->readFunction = readFunction;
	this->writeFunction = writeFunction;
	this->debug = debug;
	sprintf(debugBuffer, "InstructionHandler() -> initialized");
	dispBuffer();
}


#endif // INSTRUCTIONHANDLER_H