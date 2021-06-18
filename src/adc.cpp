#include "adc.hpp"

Adc::Adc(uint8_t address, TwoWire *wire)
    : address(address), wire(wire) {}

bool Adc::begin()
{
    wire->begin();
    return true;
}

void Adc::read()
{
    wire->requestFrom(8, 8);
    int count = 0;
    while (wire->available())
    {
        uint8_t highByte = wire->read();
        uint8_t lowByte = wire->read();
        value[count] = (uint16_t)256 * highByte + lowByte;
    }
}