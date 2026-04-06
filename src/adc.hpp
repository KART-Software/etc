#ifndef _ADC_H_
#define _ADC_H_

#include <Arduino.h>
#include <SPI.h>
#include "constants.hpp"
#include "fast_pin.hpp"

#define SPI_CS_PIN FastPin<10, true> // Teensy 4.1 SPI0 の CS ピン (Active LOW)
#define SPI_BUS SPI

#ifdef MCP3208

#define SPI_MODE_ADC SPI_MODE0
#define SPI_BIT_ORDER MSBFIRST
#define SPI_FREQUENCY 2000000

#define SPI_BASE_BITS 0b0000011000
#define SPI_NUM_SHIFTS 14

#endif // MCP3208

#ifdef ADS8688
// https://www.ti.com/jp/lit/ds/symlink/ads8688.pdf?ts=1690504622401&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252Fja-jp%252FADS8688

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

#endif

template <typename CsPin>
class _adc
{
public:
    _adc();
    void begin();
    void read();
    uint16_t value[8];

private:
    SPIClass &spi = SPI_BUS;
    SPISettings spiSettings = SPISettings(SPI_FREQUENCY, SPI_BIT_ORDER, SPI_MODE_ADC);
    const uint8_t numCh = ADC_NUM_CH;
#ifdef ADS8688
    const uint8_t chs[ADC_NUM_CH] = ADC_CHANNELS;
    void setReadChannels();
    void setReadModeAutoSeq();
    void setReadRanges();
    uint32_t createChannelSelectBits();
    uint32_t createReadProgramRegister(uint8_t addr /* 7 bits */);
    uint32_t createWriteProgramRegister(uint8_t addr /* 7 bits */, uint8_t data);
    void spiTransfer(uint32_t txData, uint32_t *rxData, uint8_t numBytes);
#endif
};

using Adc = _adc<SPI_CS_PIN>;

#endif // _ADC_H_