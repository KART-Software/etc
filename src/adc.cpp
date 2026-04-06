#include "adc.hpp"

#ifdef MCP3208

template <typename CsPin>
_adc<CsPin>::_adc() {}

template <typename CsPin>
void _adc<CsPin>::begin()
{
    spi.begin();
    CsPin::init();
}

template <typename CsPin>
void _adc<CsPin>::read()
{
    for (uint8_t i = 0; i < numCh; i++)
    {
        uint32_t command = (uint32_t(SPI_BASE_BITS) | i) << SPI_NUM_SHIFTS;
        spi.beginTransaction(spiSettings);
        CsPin::set();
        uint32_t readVal = 0;
        readVal |= (uint32_t)spi.transfer((command >> 16) & 0xFF) << 16;
        readVal |= (uint32_t)spi.transfer((command >> 8) & 0xFF) << 8;
        readVal |= (uint32_t)spi.transfer(command & 0xFF);
        CsPin::clear();
        spi.endTransaction();
        value[i] = readVal & 0xFFF;
    }
}

#endif // MCP3208

#ifdef ADS8688

template <typename CsPin>
_adc<CsPin>::_adc() {}

template <typename CsPin>
void _adc<CsPin>::begin()
{
    spi.begin();
    CsPin::init();
    setReadRanges();
    setReadChannels();
    setReadModeAutoSeq();
}

template <typename CsPin>
void _adc<CsPin>::spiTransfer(uint32_t txData, uint32_t *rxData, uint8_t numBytes)
{
    spi.beginTransaction(spiSettings);
    CsPin::set();
    uint32_t rx = 0;
    if (numBytes == 4)
    {
        rx = spi.transfer32(txData);
    }
    else
    {
        for (uint8_t i = numBytes; i > 0; i--)
        {
            rx |= (uint32_t)spi.transfer((txData >> ((i - 1) * 8)) & 0xFF) << ((i - 1) * 8);
        }
    }
    CsPin::clear();
    spi.endTransaction();
    if (rxData)
    {
        *rxData = rx;
    }
}

template <typename CsPin>
void _adc<CsPin>::read()
{
    uint32_t readVal;
    for (int i = 0; i < ADC_NUM_CH - 1; i++)
    {
        spiTransfer(NO_OP << 16, &readVal, 4);
        value[chs[i]] = readVal >> 1; // なぜか 2ビット目から17ビット目までがデータになっている（？）ので、">> 1" をつけて、16ビットにキャストしたときにちょうどになるようにしている。
    }
    spiTransfer(AUTO_RST << 16, &readVal, 4);
    value[chs[ADC_NUM_CH - 1]] = readVal >> 1;
}

template <typename CsPin>
void _adc<CsPin>::setReadChannels()
{
    uint8_t enableChannelBits = createChannelSelectBits();
    uint32_t powerDownReg = createWriteProgramRegister(CH_POWER_DOWN_ADDR, ~enableChannelBits);
    uint32_t autoSeqReg = createWriteProgramRegister(AUTO_SEQ_EN_ADDR, enableChannelBits);

    spiTransfer(powerDownReg, nullptr, 3);
    spiTransfer(autoSeqReg, nullptr, 3);
}

template <typename CsPin>
uint32_t _adc<CsPin>::createChannelSelectBits()
{
    uint8_t bits = 0;
    bits |= 1 << APPS_1_CH;
    bits |= 1 << APPS_2_CH;
    bits |= 1 << TPS_1_CH;
    bits |= 1 << TPS_2_CH;
    bits |= 1 << ITTR_CH;
    bits |= 1 << BPS_CH;
    bits |= 1 << MOTOR_CURRENT_CH;
    return bits;
}

template <typename CsPin>
void _adc<CsPin>::setReadRanges()
{
    for (uint32_t i = 0; i < 8; i++)
    {
        uint32_t reg = createWriteProgramRegister(RANGE_SELECT_ADDR_0 + i, RANGE_4);
        spiTransfer(reg, nullptr, 3);
    }
}

template <typename CsPin>
uint32_t _adc<CsPin>::createWriteProgramRegister(uint8_t addr /* 7 bits */, uint8_t data)
{
    uint32_t reg = addr & 0b1111111;
    reg = (reg << 1) | 1;
    reg = (reg << 8) | data;
    reg = reg << 8;
    return reg;
}

template <typename CsPin>
void _adc<CsPin>::setReadModeAutoSeq()
{
    spiTransfer(AUTO_RST << 16, nullptr, 4);
}

#endif // ADS8688

// 明示的インスタンス化
template class _adc<SPI_CS_PIN>;
