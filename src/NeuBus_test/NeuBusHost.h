#ifndef NEUBUSHOST_H
#define NEUBUSHOST_H
#include <Arduino.h>
#include <digitalWriteFast.h>
#include <RingBuf.h>

class NeuBusHost
{
    public:
        NeuBusHost();
        void read(uint8_t);
        void write(uint8_t);
    private:

};

#endif