#include "NeuBusHost.h"
#include <Arduino.h>
#include <digitalWriteFast.h>

#define DDR_CONTROL DDRB
#define DDR_DATA DDRD

#define PORT_CONTROL PORTB
#define PORT_DATA PORTD

#define PIN_CONTROL PINC
#define PIN_DATA PIND

const static int8_t RDY = 2;
const static int8_t EXPRDY = 3;
const static int8_t nWAIT = 4;
const static int8_t INIR = 5;
const static int8_t nRESET = 6;
const static int8_t nWRITE = 7;
const static int8_t nDATASTRB = 14;
const static int8_t nADDRSTRB = 15;

NeuBusHost::NeuBusHost()
{
    PORT_DATA = 0x00;
    DDR_DATA = 0x00; // input mode
    pinModeFast(RDY, INPUT);
    pinModeFast(EXPRDY, INPUT);
    pinModeFast(nWAIT, INPUT);
    pinModeFast(INIR, INPUT);
    pinModeFast(nRESET, OUTPUT);
    pinModeFast(nWRITE, OUTPUT);
    pinModeFast(nDATASTRB, OUTPUT);
    pinModeFast(nADDRSTRB, OUTPUT);

    digitalWriteFast(nRESET, HIGH);
    digitalWriteFast(nWRITE, HIGH);
    digitalWriteFast(nDATASTRB, HIGH);
    digitalWriteFast(nADDRSTRB, HIGH);
}

void NeuBusHost::write(uint8_t packet)
{
    PORT_DATA = packet & 0xFF;
    digitalWriteFast(nWRITE, LOW);
    DDR_DATA = 0xFF;
    digitalWriteFast(nDATASTRB, LOW);
    while(digitalReadFast(nWAIT) == LOW);
    digitalWriteFast(nDATASTRB, HIGH);
    DDR_DATA = 0x00;
    digitalWriteFast(nWRITE, HIGH);
}