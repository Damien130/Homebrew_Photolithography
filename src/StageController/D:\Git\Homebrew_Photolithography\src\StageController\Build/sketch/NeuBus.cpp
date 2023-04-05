#line 1 "/Users/damienhu/Documents/Git/Homebrew_Photolithography/src/StageController/NeuBus.cpp"
#include "NeuBus.h"
#include <Arduino.h>
#include <digitalWriteFast.h>

const static int8_t nWAIT = 26;
const static int8_t INIR = 27;
const static int8_t nRESET = 28;
const static int8_t nWRITE = 29;
const static int8_t RDY = 24;
const static int8_t EXPRDY = 25;
const static int8_t nDATASTRB = 2;
const static int8_t nADDRSTRB = 3;

#define DDR_CONTROL DDRA
#define DDR_DATA DDRC

#define PORT_CONTROL PORTA
#define PORT_DATA PORTC

#define PIN_DATA PINC

/* AVR Direct Port Manipulation:
    DDR$: Direction Register r/w
    PORT$: output state r/w
    PIN$: input register r

    Pin#   76543210
    Str:  B01010101

    Control Bus: nWRITE nRESET nINTR nWAIT nEXPRDY nRDY PSELECT1 PSELECT2
    Pin#            7      6     5     4      3      2      1       0
 */



NeuBus::NeuBus()
{
    PORT_DATA = 0x00; // data bus default state, nothing is ready yet
    PORT_CONTROL = 0x3C; // control bus, nothing is ready yet
    DDR_CONTROL = DDR_CONTROL | B00111100; // configure output
    DDR_DATA = 0xFF; // PORT Data, default output mode
    
}

NeuBus::NeuBus(int8_t nwrite, int8_t nreset, int8_t nintr, int8_t nwait, 
               int8_t nexprdy, int8_t nrdy, int8_t pselect1, int8_t pselect2,
               volatile uint8_t &DDR, volatile uint8_t &PORT)
{
    pinModeFast(nwrite, INPUT);
    pinModeFast(nreset, INPUT);
    pinModeFast(nintr, OUTPUT);
    pinModeFast(nwait, OUTPUT);
    pinModeFast(nexprdy, OUTPUT);
    pinModeFast(nrdy, OUTPUT);
    pinModeFast(pselect1, INPUT);
    pinModeFast(pselect2, INPUT);
    PORT = 0x00;
    DDR = 0xFF;
}


void NeuBus::deviceReady()
{
    digitalWriteFast(RDY, HIGH);
}

void NeuBus::deviceNotReady()
{
    digitalWriteFast(RDY, LOW);
}

void NeuBus::readyToExpose()
{
    digitalWriteFast(EXPRDY, HIGH);

}

void NeuBus::notReadyToExpose()
{
    digitalWriteFast(EXPRDY, LOW);
}

void NeuBus::sendData(uint8_t packet, volatile bool &DataStrobe)
{
    PORT_DATA = packet & 0xFF; // put packet in register
    DDR_DATA = 0xFF; // put packet on bus
    digitalWriteFast(nWAIT, HIGH); // OK to end cycle
    while(digitalReadFast(nDATASTRB == LOW)); // wait until host acknoledges
    DataStrobe = false;
    digitalWriteFast(nWAIT, LOW); // end cycle
    DDR_DATA = 0x00; // detach from bus
}

void NeuBus::sendAddress(uint8_t packet, volatile bool &AddressStrobe)
{
    PORT_DATA = packet & 0xFF; // put packet in register
    DDR_DATA = 0xFF; // put packet on bus
    digitalWriteFast(nWAIT, HIGH); // OK to end cycle
    while(digitalReadFast(nADDRSTRB == LOW)); // wait until host acknoledges
    AddressStrobe = false;
    digitalWriteFast(nWAIT, LOW); // end cycle
    DDR_DATA = 0x00; // detach from bus
}

uint8_t NeuBus::readData(volatile bool &DataStrobe)
{
    uint8_t packet = PIN_DATA;
    digitalWriteFast(nWAIT, HIGH); // OK to end cycle
    while(digitalReadFast(nDATASTRB == LOW)); // wait until host acknoledges
    DataStrobe = false;
    digitalWriteFast(nWAIT, LOW); // end cycle
    return packet;
}

uint8_t NeuBus::readAddress(volatile bool &AddressStrobe)
{
    uint8_t packet = PIN_DATA;
    digitalWriteFast(nWAIT, HIGH); // OK to end cycle
    while(digitalReadFast(nADDRSTRB == LOW)); // wait until host acknoledges
    AddressStrobe = false;
    digitalWriteFast(nWAIT, LOW); // end cycle
    return packet;
}