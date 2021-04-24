#include "tps.hpp"

double readTps1()
{
    double value = analogRead(TPS_1_PIN);
    double convertedValue = value * TPS_SLOPE_1 + TPS_INTERCEPT_1;
    if (convertedValue < TPS_MIN_1 || convertedValue > TPS_MAX_1)
    {
        gErrorHandler.raise(ERR_TPS_1_OUT_OF_RANGE);
    }
    return convertedValue;
}

double readTps2()
{
    double value = analogRead(TPS_2_PIN);
    double convertedValue = value * TPS_SLOPE_2 + TPS_INTERCEPT_2;
    if (convertedValue < TPS_MIN_2 || convertedValue > TPS_MAX_2)
    {
        gErrorHandler.raise(ERR_TPS_2_OUT_OF_RANGE);
    }
    return convertedValue;
}