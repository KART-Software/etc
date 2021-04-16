#include <Arduino.h>
#include <math.h>
#include "tps.hpp"

Tps::Tps(uint8_t inputPin1, uint8_t inputPin2, double ratio, double offset) : inputPin1(inputPin1),
                                                                              inputPin2(inputPin2),
                                                                              ratio(ratio),
                                                                              offset(offset)
{
}

void Tps::readTps()
{
    tpsData.rawValue1 = analogRead(inputPin1);
    tpsData.rawValue2 = analogRead(inputPin2);
    tpsData.value = (tpsData.rawValue1 + tpsData.rawValue2) * ratio + offset;
    checkPlausibility();
}

void Tps ::checkPlausibility()
{
    if (tpsData.rawValue1 * 0.05 < abs(tpsData.rawValue1 - tpsData.rawValue2))
    { // TODO check
        gErrorHandler.raise(ERR_TPS_IMPLAUSIBLE);
    }
}