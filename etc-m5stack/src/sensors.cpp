#include "sensors.hpp"

Sensor::Sensor(double minValue, double maxValue, double margin, int16_t rawMinValue, int16_t rawMaxValue)
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

Apps::
    Apps(double minValue, double maxValue, double margin, int16_t rawMinValue, int16_t rawMaxValue, uint8_t pin)
    : Sensor(minValue, maxValue, margin, rawMinValue, rawMaxValue),
      pin(pin)
{
}

double Apps::read()
{
    // rawValue = analogRead(pin);
    rawValue = gAdc.value[pin];
    return validatedConvertedValue();
}

double Apps::convertToTargetTp()
{
    double app = convertedValue();
    return app;

    // double X1, Y1, X2, Y2;
    // X1 = 60;
    // Y1 = 15;
    // X2 = 87;
    // Y2 = 41;

    // if (app <= 0.0)
    // {
    //     return 0.0;
    // }
    // else if (app < X1)
    // {
    //     return map(app, 0.0, X1, 0.0, Y1);
    // }
    // else if (app < X2)
    // {
    //     return map(app, X1, X2, Y1, Y2);
    // }
    // else if (app < 100.0)
    // {
    //     return map(app, X2, 100.0, Y2, 100.0);
    // }
    // else
    // {
    //     return 100.0;
    // }
}

Tps::
    Tps(double minValue, double maxValue, double margin, int16_t rawMinValue, int16_t rawMaxValue, uint8_t pin)
    : Sensor(minValue, maxValue, margin, rawMinValue, rawMaxValue),
      pin(pin)
{
}

double Tps::read()
{
    // rawValue = analogRead(pin);
    rawValue = gAdc.value[pin];
    return validatedConvertedValue();
}
