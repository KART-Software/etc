#ifndef _ADC_H_
#define _ADC_H_

#include <Arduino.h>
#include <SPI.h>
#include "constants.hpp"

// https://www.ti.com/jp/lit/ds/symlink/ads8688.pdf

#define SPI_MODE_ADC SPI_MODE1
#define SPI_BIT_ORDER MSBFIRST
#define SPI_FREQUENCY 5000000

#define AUTO_SEQ_EN_ADDR 0x01
#define CH_POWER_DOWN_ADDR 0x02

/// Registers
#define NO_OP 0x0000
#define AUTO_RST 0xA000

/// Range Select

#define RANGE_SELECT_ADDR_0 0x05
#define RANGE_SELECT_ADDR_1 0x06
#define RANGE_SELECT_ADDR_2 0x07
#define RANGE_SELECT_ADDR_3 0x08
#define RANGE_SELECT_ADDR_4 0x09
#define RANGE_SELECT_ADDR_5 0x0A
#define RANGE_SELECT_ADDR_6 0x0B
#define RANGE_SELECT_ADDR_7 0x0C

#define RANGE_0 0b0000 // ±2.5 x VREF
#define RANGE_1 0b0001 // ±1.25 x VREF
#define RANGE_2 0b0010 // ±0.625 x VREF
#define RANGE_3 0b0101 // 0 ~ 2.5 x VREF
#define RANGE_4 0b0110 // 0 ~ 1.25 x VREF
                       // VREF = 4.096V

class Adc
{
public:
    Adc(uint8_t csPin, SPIClass &spi = SPI);
    void begin();
    void read();
    uint16_t value[8];
    uint32_t sps() const { return intervalUs_ ? 1000000u / intervalUs_ : 0; }

private:
    SPIClass &spi;
    uint8_t csPin;
    SPISettings spiSettings = SPISettings(SPI_FREQUENCY, SPI_BIT_ORDER, SPI_MODE_ADC);
    const uint8_t numCh = ADC_NUM_CH;
    const uint8_t chs[ADC_NUM_CH] = ADC_CHANNELS;
    uint32_t intervalUs_ = 0;
    uint32_t lastReadUs_ = 0;
    void setReadChannels();
    void setReadModeAutoSeq();
    void setReadRanges();
    uint32_t createChannelSelectBits();
    void writeRegister(uint8_t addr, uint8_t value);
    uint32_t transferCommand32(uint16_t cmd);
};

#endif // _ADC_H_