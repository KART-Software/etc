#include "sensors.hpp"

Sensor::Sensor(uint16_t rawMinValue, uint16_t rawMaxValue, double minValue, double maxValue, double margin)
    : rawMinValue(rawMinValue), rawMaxValue(rawMaxValue), minValue(minValue), maxValue(maxValue), margin(margin)
{
    setConversion(minValue, maxValue);
}

void Sensor::setConversion(double minValue, double maxValue)
{
    this->slope = (maxValue - minValue) / (rawMaxValue - rawMinValue);
    this->intercept = (rawMaxValue * minValue - rawMinValue * maxValue) / (rawMaxValue - rawMinValue);
}

double Sensor::convertedValue()
{
    return rawValue * slope + intercept;
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
    Apps(uint16_t rawMinValue, uint16_t rawMaxValue, uint8_t ch, double minValue, double maxValue, double margin, double idlingValue)
    : Sensor(rawMinValue, rawMaxValue, minValue, maxValue, margin),
      ch(ch), idlingValue(idlingValue)
{
    setIdling(true);
}

void Apps::read()
{
    rawValue = gAdc.value[ch];
}

double Apps::convertToTargetTp()
{
    double app = convertedValue();
    return constrain(app, getMinValue(), getMaxValue()); // TODO change
}

void Apps::setIdling(bool idling)
{
    if (idling)
    {
        setConversion(idlingValue, maxValue);
    }
    else
    {
        setConversion(minValue, maxValue);
    }
}

Tps::
    Tps(uint16_t rawMinValue, uint16_t rawMaxValue, uint8_t ch, double minValue, double maxValue, double margin)
    : Sensor(rawMinValue, rawMaxValue, minValue, maxValue, margin),
      ch(ch)
{
}

void Tps::read()
{
    rawValue = gAdc.value[ch];
}

Ittr::
    Ittr(uint16_t rawMinValue, uint16_t rawMaxValue, uint8_t ch, double minValue, double maxValue, double margin, double idlingValue)
    : Apps(rawMinValue, rawMaxValue, ch, minValue, maxValue, margin, idlingValue)
{
}

Bps::Bps(uint16_t rawMinValue, uint16_t rawMaxValue, uint8_t ch, double minValue, double maxValue, double highPressureThreshold, double margin)
    : Sensor(rawMinValue, rawMaxValue, minValue, maxValue, margin),
      ch(ch), highPressureThreshold(highPressureThreshold)
{
}

void Bps::read()
{
    rawValue = gAdc.value[ch];
}

bool Bps::isHighPressure()
{
    return convertedValue() > highPressureThreshold;
}