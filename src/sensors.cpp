#include "sensors.hpp"

Sensor::Sensor(uint16_t rawMinValue, uint16_t rawMaxValue, double minValue, double maxValue, double margin)
    : rawMinValue(rawMinValue), rawMaxValue(rawMaxValue), minValue(minValue), maxValue(maxValue), margin(margin)
{
    setConversion(minValue, maxValue);
}

void Sensor::setRawMin(uint16_t val)
{
    rawMinValue = val;
    setConversion();
}

void Sensor::setRawMax(uint16_t val)
{
    rawMaxValue = val;
    setConversion();
}

uint16_t Sensor::setCurrentValRawMin()
{
    rawMinValue = rawValue;
    setConversion();
    return rawMinValue;
}

uint16_t Sensor::setCurrentValRawMax()
{
    rawMaxValue = rawValue;
    setConversion();
    return rawMaxValue;
}

void Sensor::setConversion(double minValue, double maxValue)
{
    this->slope = (maxValue - minValue) / (rawMaxValue - rawMinValue);
    this->intercept = (rawMaxValue * minValue - rawMinValue * maxValue) / (rawMaxValue - rawMinValue);
}

void Sensor::setConversion()
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
    double x = constrain(app, getMinValue(), getMaxValue());
    double y = -0.0000007403 * x * x * x * x + 0.0001425457 * x * x * x + 0.0025399794 * x * x + 0.0608039592 * x; // TODO change
    if (idling)
    {
        return idlingValue + y * (100.0 - idlingValue) / 100.0;
    }
    return y;
}

void Apps::setIdling(bool idling)
{
    this->idling = idling;
}

Tps::
    Tps(uint16_t rawMinValue, uint16_t rawMaxValue, uint8_t ch, double minValue, double maxValue, double largeOpenThreshold, double margin)
    : Sensor(rawMinValue, rawMaxValue, minValue, maxValue, margin),
      ch(ch), largeOpenThreshold(largeOpenThreshold)
{
}

void Tps::read()
{
    rawValue = gAdc.value[ch];
}

bool Tps::isLargeOpen()
{
    return convertedValue() > largeOpenThreshold;
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