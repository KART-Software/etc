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

#ifdef ADS8688

Adc::Adc()
{
}

void Adc::begin()
{
    spi.begin();
    spi.setFrequency(SPI_FREQUENCY);
    spi.setDataMode(SPI_MODE);
    spi.setBitOrder(SPI_BIT_ORDER);
    spi.setHwCs(true);
    setReadRanges();
    setReadChannels();
    setReadModeAutoSeq();
}

void Adc::read()
{
    uint32_t readVal;
    for (int i = 0; i++; i < ADC_NUM_CH - 1)
    {
        spi.transferBits(NO_OP, &readVal, NUM_COMMAND_REGISTER_BITS);
        value[chs[i]] = readVal;
    }
    spi.transferBits(AUTO_RST, &readVal, NUM_COMMAND_REGISTER_BITS);
    value[chs[ADC_NUM_CH - 1]] = readVal;
}

void Adc::setReadChannels()
{
    uint8_t enableChnnelBits = createChannelSelectBits();
    uint32_t powerDowmReg = createWriteProgramRegister(CH_POWER_DOWN_ADDR, ~enableChnnelBits);
    uint32_t autoSeqReg = createWriteProgramRegister(AUTO_SEQ_EN_ADDR, enableChnnelBits);

    spi.transferBits(powerDowmReg, NULL, NUM_PROGRAM_REGISTER_BITS);
    spi.transferBits(autoSeqReg, NULL, NUM_PROGRAM_REGISTER_BITS);
}

uint32_t Adc::createChannelSelectBits()
{
    uint32_t bits = 0;
    bits &= 1 << APPS_1_CH;
    bits &= 1 << APPS_2_CH;
    bits &= 1 << TPS_1_CH;
    bits &= 1 << TPS_2_CH;
    bits &= 1 << ITTR_CH;
    bits &= 1 << BSE_CH;
    bits &= 1 << MOTOR_CURRENT_CH;
    return bits;
}

void Adc::setReadRanges()
{
    for (uint32_t i = 0; i < 8; i++)
    {
        uint32_t reg = createWriteProgramRegister(RANGE_SELECT_ADDR_0 + i, RANGE_4);
        spi.transferBits(reg, NULL, NUM_PROGRAM_REGISTER_BITS);
    }
}

uint32_t Adc::createWriteProgramRegister(uint8_t addr /* 7 bits */, uint8_t data)
{
    uint32_t reg = addr & 0b1111111;
    reg = (reg << 1) | uint8_t(1);
    reg = (reg << 8) | data;
    return reg;
}

void Adc::setReadModeAutoSeq()
{
    spi.transferBits(AUTO_RST, NULL, NUM_COMMAND_REGISTER_BITS);
}

#endif
