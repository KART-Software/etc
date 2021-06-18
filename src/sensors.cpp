#include "sensors.hpp"

Sensor::Sensor(double minValue, double maxValue, double intercept, double slope)
{
    setRange(minValue, maxValue);
    setConversion(intercept, slope);
}

void Sensor::setRange(double minValue, double maxValue)
{
    this->minValue = minValue;
    this->maxValue = maxValue;
}

void Sensor::setConversion(double intercept, double slope)
{
    this->intercept = intercept;
    this->slope = slope;
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

Apps::
    Apps(double minValue, double maxValue, double intercept, double slope, uint8_t pin)
    : Sensor(minValue, maxValue, intercept, slope),
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
    Tps(double minValue, double maxValue, double intercept, double slope, uint8_t pin)
    : Sensor(minValue, maxValue, intercept, slope),
      pin(pin)
{
}

double Tps::read()
{
    // rawValue = analogRead(pin);
    rawValue = gAdc.value[pin];
    return validatedConvertedValue();
}
