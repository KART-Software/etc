#ifndef _ADC_H_
#define _ADC_H_

#include <M5Stack.h>
#include <Wire.h>

#define ADC_ARDUINO_ADDRESS 8
#define READ_BYTE_LENGTH 10
#define READ_DATA_LENGTH 5

class Adc
{
public:
    Adc(uint8_t address, TwoWire *wire = &Wire);
    bool begin();
    void read();
    int16_t value[READ_DATA_LENGTH];

private:
    const uint8_t address;
    TwoWire *wire;
};

#endif