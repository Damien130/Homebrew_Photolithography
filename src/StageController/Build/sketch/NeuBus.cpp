#line 1 "D:\\Git\\Homebrew_Photolithography\\src\\StageController\\NeuBus.cpp"
#include "NeuBus.h"
#include <Arduino.h>
#include <digitalWriteFast.h>

const static int8_t IOCRDY = 47;
const static int8_t RDY = 48;
const static int8_t INPOS = 49;
const static int8_t EXPRDY = 50;

#define DDR_CONTROL DDRD
#define DDR_ADDRESS DDRA
#define DDR_DATA DDRC

#define PORT_CONTROL PORTD
#define PORT_ADDRESS PORTA
#define PORT_DATA PORTC

#define PIN_ADDRESS PINA
#define PIN_DATA PINC

/* AVR Direct Port Manipulation:
    DDR$: Direction Register r/w
    PORT$: output state r/w
    PIN$: input register r

    Pin#   76543210
    Str:  B01010101
 */
NeuBus::NeuBus()
{
    // default constructor, D control, A Address, C data
    PORT_DATA = 0x00; // data bus default state, nothing is ready yet
    PORT_CONTROL = 0x00; // control bus, nothing is ready yet
    DDR_CONTROL = DDR_CONTROL | B11110000; // PORTD 4 - PORTD 7 configure to output
    DDR_ADDRESS = 0x00; // PORT A as input
    DDR_DATA = 0xFF; // PORT C, default output mode
    
    addressLatched = 0;
}

void NeuBus::deviceNotReady()
{
    digitalWriteFast(RDY, LOW);
}

void NeuBus::deviceReady()
{
    digitalWriteFast(RDY, HIGH);
}

void NeuBus::inPosition(bool inPos)
{
    if (inPos)
    {
        digitalWriteFast(INPOS, HIGH);
    } else {
        digitalWriteFast(INPOS, LOW);
    }
}

void NeuBus::readyToExpose(bool rdyExp)
{
    if (rdyExp)
    {
        digitalWriteFast(EXPRDY, HIGH);
    } else {
        digitalWriteFast(EXPRDY, LOW);
    }
}

void NeuBus::send(uint8_t packet)
{
    digitalWriteFast(IOCRDY, LOW); // Pull down IOCHRDY
    DDR_DATA = 0xFF; // output mode
    PORT_DATA = packet; // put packet on the bus
    digitalWriteFast(IOCRDY, HIGH); // signal IO Channel RDY, rPi processes IRQ and reads
}


void NeuBus::send(uint16_t packet, volatile bool &ALE)
{
    uint8_t lower = packet & 0xFF;   // split the 16 bit # in two
    uint8_t higher = (packet >> 8) & 0xFF; // & 0xFF to "fast cast"

    switch (ALE)
    {
        case 0:
            // if ALE not enabled, send them consecutively
            send(higher);
            send(lower);
            break;
        
        case 1:
            // if ALE enabled, send as one 16 bit #
            digitalWriteFast(IOCRDY, LOW); // Pull down IOCHRDY
            DDR_ADDRESS = 0xFF; // set port A to output
            DDR_DATA = 0xFF; // output mode
            PORT_ADDRESS = higher; // put packet on the bus
            PORT_DATA = lower; // put packet on the bus
            digitalWriteFast(IOCRDY, HIGH); // signal IO Channel RDY, rPi processes IRQ and reads
            DDR_ADDRESS = 0x00; // PORT A as input
            break;
        default:
            break;
    }
}

void NeuBus::send(uint32_t packet, volatile bool &ALE)
{
    uint8_t packet1 = packet & 0xFF;
    uint8_t packet2 = (packet >> 8) & 0xFF;
    uint8_t packet3 = (packet >> 16) & 0xFF;
    uint8_t packet4 = (packet >> 24) & 0xFF; // large packet

    switch (ALE)
    {
        case 0:
            // if ALE not enabled, send consecutively
            send(packet4);
            send(packet3);
            send(packet2);
            send(packet1);
            break;
        case 1:
            // if ALE enabled, send as two 16 bit #
            digitalWriteFast(IOCRDY, LOW); // Pull down IOCHRDY
            DDR_ADDRESS = B11111111; // set port A to output
            DDR_DATA = B11111111; // output mode
            PORT_ADDRESS = packet4; // put packet on the bus
            PORT_ADDRESS = packet3; // put packet on the bus
            digitalWriteFast(IOCRDY, HIGH); // signal IO Channel RDY, rPi processes IRQ and reads
            digitalWriteFast(IOCRDY, LOW); // Pull down IOCHRDY
            PORT_ADDRESS = packet2; // put packet on the bus
            PORT_ADDRESS = packet1; // put packet on the bus
            digitalWriteFast(IOCRDY, HIGH); // signal IO Channel RDY, rPi processes IRQ and reads
            DDR_ADDRESS = B00000000; // PORT A as input
            break;
        default:
            break;
    }
}

uint8_t NeuBus::read()
{
    uint8_t packet = PIN_DATA;
    return packet;
}