#include <Arduino.h>
#include <math.h>
#include "tps.hpp"

int tps(uint8_t inputPin, int ratio, int offset)
{
    uint8_t input = analogRead(inputPin);
    return input * ratio + offset;
}

Tps::Tps(uint8_t inputPin1, uint8_t inputPin2, float ratio, float offset) : inputPin1(inputPin1),
                                                                            inputPin2(inputPin2),
                                                                            ratio(ratio),
                                                                            offset(offset)
{
}

//TpsData Tps::getTpsData()
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
    }
}