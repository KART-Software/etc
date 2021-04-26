#include "sensors.hpp"

double readAnalogSensor(uint8_t pin, double slope, double intercept, double min, double max, uint8_t errID)
{
    double value = analogRead(pin);
    double convertedValue = value * slope + intercept;
    if (convertedValue < min || convertedValue > max)
    {
        gErrorHandler.raise(errID);
    }
    if (convertedValue < min)
    {
        convertedValue = min;
        gErrorHandler.raise(errID);
    }
    else if (convertedValue > max)
    {
        convertedValue = max;
        gErrorHandler.raise(errID);
    }
    return convertedValue;
}

double readTps1()
{
    return readAnalogSensor(
        TPS_1_PIN, TPS_1_SLOPE,
        TPS_1_INTERCEPT,
        TPS_1_MIN,
        TPS_1_MAX,
        ERR_TPS_1_CIRCUIT_FAILURE);
}

double readTps2()
{
    return readAnalogSensor(
        TPS_2_PIN, TPS_2_SLOPE,
        TPS_2_INTERCEPT,
        TPS_2_MIN,
        TPS_2_MAX,
        ERR_TPS_2_CIRCUIT_FAILURE);
}

double readApps1()
{
    return readAnalogSensor(
        APPS_1_PIN, APPS_1_SLOPE,
        APPS_1_INTERCEPT,
        APPS_1_MIN,
        APPS_1_MAX,
        ERR_APPS_1_CIRCUIT_FAILURE);
}

double readApps2()
{
    return readAnalogSensor(
        APPS_2_PIN, APPS_2_SLOPE,
        APPS_2_INTERCEPT,
        APPS_2_MIN,
        APPS_2_MAX,
        ERR_APPS_2_CIRCUIT_FAILURE);
}