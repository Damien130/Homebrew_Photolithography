#include <Arduino.h>
#include <digitalWriteFast.h>
#include "NeuBusHost.h"

uint8_t stuff = 0;
NeuBusHost Bus;

void setup()
{

}

void loop()
{
    Bus.write(stuff);
    stuff++;
}