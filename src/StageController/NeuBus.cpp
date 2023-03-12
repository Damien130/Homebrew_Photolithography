#include "NeuBus.h"
#include <Arduino.h>
#include <digitalWriteFast.h>

const static int8_t IOCRDY = 47;
const static int8_t RDY = 48;
const static int8_t INPOS = 49;
const static int8_t EXPRDY = 50;

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
    DDRD = DDRD | B11110000; // PORTD 4 - PORTD 7 configure to output
    DDRA = B00000000; // PORT A as input
    DDRC = B11111111; // PORT C, default output mode
    PORTD = B00000000; // default state, nothing is ready yet
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
    DDRC = B11111111; // output mode
    PORTC = packet; // put packet on the bus
    digitalWriteFast(IOCRDY, HIGH); // signal IO Channel RDY, rPi processes IRQ and reads
}

inline void NeuBus::send16BitMode(uint8_t packet)
{
    digitalWriteFast(IOCRDY, LOW); // Pull down IOCHRDY
    DDRC = B11111111; // output mode
    PORTA = packet; // put packet on the bus
    digitalWriteFast(IOCRDY, HIGH); // signal IO Channel RDY, rPi processes IRQ and reads
}

void NeuBus::send(uint16_t packet, volatile bool &ALE)
{
    uint8_t lower = 0;
    uint8_t higher = 0; // packets
    lower = (uint8_t) packet;   // split the 16 bit # in two
    higher = (uint8_t)(packet >> 8);
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
            DDRA = B11111111; // set port A to output
            DDRC = B11111111; // output mode
            PORTA = higher; // put packet on the bus
            PORTA = lower; // put packet on the bus
            digitalWriteFast(IOCRDY, HIGH); // signal IO Channel RDY, rPi processes IRQ and reads
            DDRA = B00000000; // PORT A as input
            break;
        default:
            break;
    }
}

void NeuBus::send(uint32_t packet, volatile bool &ALE)
{
    uint8_t packet1 = (uint8_t) packet;
    uint8_t packet2 = (uint8_t)(packet >> 8);
    uint8_t packet3 = (uint8_t)(packet >> 16);
    uint8_t packet4 = (uint8_t)(packet >> 24); // large packet
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
            DDRA = B11111111; // set port A to output
            DDRC = B11111111; // output mode
            PORTA = packet4; // put packet on the bus
            PORTA = packet3; // put packet on the bus
            digitalWriteFast(IOCRDY, HIGH); // signal IO Channel RDY, rPi processes IRQ and reads
            digitalWriteFast(IOCRDY, LOW); // Pull down IOCHRDY
            PORTA = packet2; // put packet on the bus
            PORTA = packet1; // put packet on the bus
            digitalWriteFast(IOCRDY, HIGH); // signal IO Channel RDY, rPi processes IRQ and reads
            DDRA = B00000000; // PORT A as input
            break;
        default:
            break;
    }
}

uint8_t NeuBus::read()
{
    uint8_t packet = PINC;
    return packet;
}