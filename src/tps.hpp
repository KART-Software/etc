#ifndef _TPS_H_
#define _TPS_H_
#include <Arduino.h>
#include "globals.hpp"

class Tps
{
public:
    Tps(uint8_t inputPin1, uint8_t inputPin2, double ratio, double offset);
    const uint8_t inputPin1;
    const uint8_t inputPin2;
    const double ratio;
    const double offset;
    uint8_t getValue();
    TpsData getTpsData();
    void readTps();

private:
    TpsData tpsData;
    void checkPlausibility();
};

typedef struct
{
    uint8_t rawValue1;
    uint8_t rawValue2;
    uint8_t value; // [degree]
    TpsStatus status;
} TpsData;

enum struct TpsStatus : int
{
    Normal,
    Abnormal
};
#endif