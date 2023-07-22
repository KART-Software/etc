#include "adc.hpp"
#ifdef MCP3208
Adc::Adc() {}

void Adc::begin()
{
    spi.begin();
    spi.setFrequency(SPI_FREQUENCY);
    spi.setDataMode(SPI_MODE);
    spi.setBitOrder(SPI_BIT_ORDER);
    spi.setHwCs(true);
}

void Adc::read()
{
    uint32_t readVal;
    for (uint8_t i = 0; i < numCh; i++)
    {
        spi.transferBits((uint32_t(SPI_BASE_BITS) | i) << SPI_NUM_SHIFTS, &readVal, SPI_NUM_BITS);
        value[i] = readVal & 0xFFF;
    }
}

#endif