#include "sensors.hpp"

Sensor::Sensor(double minValue, double maxValue, int16_t rawMinValue, int16_t rawMaxValue)
{
    setRange(minValue, maxValue);
    setConversion(rawMinValue, rawMaxValue);
}

void Sensor::setRange(double minValue, double maxValue)
{
    this->minValue = minValue;
    this->maxValue = maxValue;
}

void Sensor::setConversion(int16_t rawMinValue, int16_t rawMaxValue)
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
    if (value < minValue || value > maxValue)
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

Apps::
    Apps(double minValue, double maxValue, int16_t rawMinValue, int16_t rawMaxValue, uint8_t pin)
    : Sensor(minValue, maxValue, rawMinValue, rawMaxValue),
      pin(pin)
{
}

double Apps::read()
{
    // rawValue = analogRead(pin);
    rawValue = gAdc.value[pin];
    return validatedConvertedValue();
}

Tps::
    Tps(double minValue, double maxValue, int16_t rawMinValue, int16_t rawMaxValue, uint8_t pin)
    : Sensor(minValue, maxValue, rawMinValue, rawMaxValue),
      pin(pin)
{
}

double Tps::read()
{
    // rawValue = analogRead(pin);
    rawValue = gAdc.value[pin];
    return validatedConvertedValue();
}
