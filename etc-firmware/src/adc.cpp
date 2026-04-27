#include "adc.hpp"

Adc::Adc(uint8_t csPin, SPIClass &spi) : csPin(csPin), spi(spi) {}

void Adc::begin()
{
    spi.begin();
    *(portConfigRegister(csPin)) = 3;
    setReadRanges();
    setReadChannels();
    setReadModeAutoSeq();
}

void Adc::writeRegister(uint8_t addr, uint8_t value)
{
    spi.beginTransaction(spiSettings);
    IMXRT_LPSPI4_S.TCR = (IMXRT_LPSPI4_S.TCR & 0xFFFFF000) | LPSPI_TCR_FRAMESZ(23);
    uint32_t data = ((uint32_t)((addr << 1) | 0x01) << 16) | ((uint32_t)value << 8);
    IMXRT_LPSPI4_S.TDR = data;
    while (IMXRT_LPSPI4_S.RSR & LPSPI_RSR_RXEMPTY) {}
    (void)IMXRT_LPSPI4_S.RDR;
    spi.endTransaction();
    delayMicroseconds(2);
}

uint32_t Adc::transferCommand32(uint16_t cmd)
{
    spi.beginTransaction(spiSettings);
    IMXRT_LPSPI4_S.TCR = (IMXRT_LPSPI4_S.TCR & 0xFFFFF000) | LPSPI_TCR_FRAMESZ(31);
    IMXRT_LPSPI4_S.TDR = (uint32_t)cmd << 16;
    while (IMXRT_LPSPI4_S.RSR & LPSPI_RSR_RXEMPTY) {}
    uint32_t result = IMXRT_LPSPI4_S.RDR;
    spi.endTransaction();
    delayMicroseconds(2);
    return result;
}

void Adc::read()
{
    uint32_t now = micros();
    if (lastReadUs_)
    {
        uint32_t delta = now - lastReadUs_;
        // EMA: interval = (interval * 7 + delta) / 8
        intervalUs_ = intervalUs_ ? (intervalUs_ * 7 + delta) >> 3 : delta;
    }
    lastReadUs_ = now;

    uint32_t readVal;
    for (int i = 0; i < ADC_NUM_CH - 1; i++)
    {
        readVal = transferCommand32(NO_OP);
        value[chs[i]] = static_cast<uint16_t>(readVal);
    }
    readVal = transferCommand32(AUTO_RST);
    value[chs[ADC_NUM_CH - 1]] = static_cast<uint16_t>(readVal);
}

void Adc::setReadChannels()
{
    uint8_t enableChannelBits = createChannelSelectBits();
    writeRegister(CH_POWER_DOWN_ADDR, ~enableChannelBits);
    writeRegister(AUTO_SEQ_EN_ADDR, enableChannelBits);
}

uint32_t Adc::createChannelSelectBits()
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

void Adc::setReadRanges()
{
    for (uint32_t i = 0; i < 8; i++)
    {
        writeRegister(RANGE_SELECT_ADDR_0 + i, RANGE_4);
    }
}

void Adc::setReadModeAutoSeq()
{
    transferCommand32(AUTO_RST);
}
