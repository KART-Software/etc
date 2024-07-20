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
    double raw = mvgAvg.getAvg();
    return raw * slope + intercept;
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

void Apps::read()
{
    rawValue = gAdc.value[ch];
    mvgAvg.add(rawValue);
}

double Apps::constrainedValue()
{
    return constrain(convertedValue(), minValue, maxValue);
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
    mvgAvg.add(rawValue);
}

bool Tps::isLargeOpen()
{
    return convertedValue() > largeOpenThreshold;
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

void Bps::read()
{
    rawValue = gAdc.value[ch];
    mvgAvg.add(rawValue);
}

bool Bps::isHighPressure()
{
    return convertedValue() > highPressureThreshold;
}

Target::Target(Apps &apps, Ittr &ittr)
    : apps(apps), ittr(ittr)
{
    setModeNormal();
}

bool Target::isIttr()
{
    return _isIttr;
}

void Target::setIttr(bool isIttr)
{
    _isIttr = isIttr;
}

double Target::getTarget()
{
    if (_isManual)
    {
        return manualTarget;
    }
    double x;
    if (_isIttr)
    {
        x = ittr.constrainedValue();
    }
    else
    {
        x = apps.constrainedValue();
    }
    double y = -0.0000007403 * x * x * x * x + 0.0001425457 * x * x * x + 0.0025399794 * x * x + 0.0608039592 * x; // TODO change
    return minValue + y * (maxValue - minValue) / 100.0;                                                           // TODO change
}

void Target::setModeCalibration()
{
    mode = Mode::Calibration;
    minValue = tpsMinValue;
    maxValue = tpsMaxValue;
}

void Target::setModeNormal()
{
    mode = Mode::Normal;
    minValue = idlingValue;
    maxValue = normalMaxValue;
}

void Target::setModeRestricted()
{
    mode = Mode::Restricted;
    minValue = idlingValue;
    maxValue = restrictedMaxValue;
}

void Target::setIdlingValue(double val)
{
    idlingValue = val;
}

void Target::read()
{
    if (_isIttr)
    {
        ittr.read();
    }
    else
    {
        apps.read();
    }
}

uint16_t Target::getSensorRawValue()
{
    if (_isIttr)
    {
        return ittr.getRawValue();
    }
    else
    {
        return apps.getRawValue();
    }
}

double Target::getSensorValue()
{
    if (_isIttr)
    {
        return ittr.convertedValue();
    }
    else
    {
        return apps.convertedValue();
    }
}

const char *Target::getModeString()
{
    switch (mode)
    {
    case Target::Mode::Calibration:
        return "Calibration";
    case Target::Mode::Normal:
        return "Normal";
    case Target::Mode::Restricted:
        return "Restricted";
    default:
        return "";
    }
}

bool Target::setManual()
{
    if (!_isManual)
    {
        manualTarget = ((int)(getTarget() * 10.0)) * 0.1; // xx.x の値に丸める
    }
    _isManual = !_isManual;
    return _isManual;
}

bool Target::isManual()
{
    return _isManual;
}

double Target::manualPlus()
{
    manualTarget += 0.1;
    return manualTarget;
}

double Target::manualMinus()
{
    manualTarget -= 0.1;
    return manualTarget;
}