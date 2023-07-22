#ifndef _ADC_H_
#define _ADC_H_

#include <Arduino.h>
#include <SPI.h>
#include "constants.hpp"

#ifdef MCP3208

#define SPI_BUS VSPI
#define SPI_MODE SPI_MODE0
#define SPI_BIT_ORDER MSBFIRST
#define SPI_FREQUENCY 2000000

#define SPI_BASE_BITS 0b0000011000
#define SPI_NUM_BITS 24
#define SPI_NUM_SHIFTS 14
#endif

#ifdef ADS8688
#define SPI_BUS VSPI
#define SPI_MODE SPI_MODE1
#define SPI_BIT_ORDER MSBFIRST
#define SPI_FREQUENCY 5000000
#endif
class Adc
{
public:
    Adc();
    void begin();
    void read();
    uint16_t value[ADC_NUM_CH];

private:
    SPIClass spi = SPIClass(VSPI);
    const uint8_t numCh = ADC_NUM_CH;
};
#endif