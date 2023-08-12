#include "sensors.hpp"

Sensor::Sensor(uint16_t rawMinValue, uint16_t rawMaxValue, double minValue, double maxValue, double margin)
    : margin(margin)
{
    setRange(minValue, maxValue);
    setConversion(rawMinValue, rawMaxValue);
}

void Sensor::setRange(double minValue, double maxValue)
{
    this->minValue = minValue;
    this->maxValue = maxValue;
}

void Sensor::setConversion(uint16_t rawMinValue, uint16_t rawMaxValue)
{
    this->slope = (maxValue - minValue) / (rawMaxValue - rawMinValue);
    this->intercept = (rawMaxValue * minValue - rawMinValue * maxValue) / (rawMaxValue - rawMinValue);
}

double Sensor::convertedValue()
{
    return rawValue * slope + intercept;
}

double Sensor::validatedConvertedValue()
{
    return constrain(convertedValue(), minValue, maxValue);
}

bool Sensor::isInRange()
{
    double value = convertedValue();
    if (value < minValue - margin || value > maxValue + margin)
    {
        return false;
    }
    return true;
}

double Sensor::getMaxValue()
{
    return maxValue;
}

double Sensor::getMinValue()
{
    return minValue;
}

uint16_t Sensor::getRawValue()
{
    return rawValue;
}

Apps::
    Apps(uint16_t rawMinValue, uint16_t rawMaxValue, uint8_t ch, double minValue, double maxValue, double margin)
    : Sensor(rawMinValue, rawMaxValue, minValue, maxValue, margin),
      ch(ch)
{
}

double Apps::read()
{
    rawValue = gAdc.value[ch];
    return validatedConvertedValue();
}

double Apps::convertToTargetTp()
{
    double app = convertedValue();
    return constrain(app, getMinValue(), getMaxValue()); // TODO change
}

Tps::
    Tps(uint16_t rawMinValue, uint16_t rawMaxValue, uint8_t ch, double minValue, double maxValue, double margin)
    : Sensor(rawMinValue, rawMaxValue, minValue, maxValue, margin),
      ch(ch)
{
}

double Tps::read()
{
    rawValue = gAdc.value[ch];
    return validatedConvertedValue();
}

Ittr::
    Ittr(uint16_t rawMinValue, uint16_t rawMaxValue, uint8_t ch, double minValue, double maxValue, double margin)
    : Apps(rawMinValue, rawMaxValue, ch, minValue, maxValue, margin)
{
}

Bps::Bps(uint16_t rawMinValue, uint16_t rawMaxValue, uint8_t ch, double minValue, double maxValue, double highPressureThreshold, double margin)
    : Sensor(rawMinValue, rawMaxValue, minValue, maxValue, margin),
      ch(ch), highPressureThreshold(highPressureThreshold)
{
}

double Bps::read()
{
    rawValue = gAdc.value[ch];
    return validatedConvertedValue();
}

bool Bps::isHighPressure()
{
    return convertedValue() > highPressureThreshold;
}