#ifndef _SENSORS_H_
#define _SENSORS_H_

#include <M5Stack.h>

#include "init_pins.hpp"
#include "globals.hpp"

#define TPS_1_SLOPE 1.0
#define TPS_2_SLOPE 1.0

#define TPS_1_INTERCEPT 0.0
#define TPS_2_INTERCEPT 0.0

#define TPS_1_RAW_MAX 1024
#define TPS_2_RAW_MAX 1024

#define TPS_1_RAW_MIN 0
#define TPS_2_RAW_MIN 0

#define TPS_1_MAX 100
#define TPS_2_MAX 100

#define TPS_1_MIN 0
#define TPS_2_MIN 0

#define APPS_1_SLOPE 1.0
#define APPS_2_SLOPE 1.0

#define APPS_1_INTERCEPT 0.0
#define APPS_2_INTERCEPT 0.0

#define APPS_1_RAW_MAX 1024
#define APPS_2_RAW_MAX 1024

#define APPS_1_RAW_MIN 0
#define APPS_2_RAW_MIN 0

#define APPS_1_MAX 100
#define APPS_2_MAX 100

#define APPS_1_MIN 0
#define APPS_2_MIN 0

class Sensor
{
public:
    Sensor();
    Sensor(double minValue, double maxValue, uint16_t rawMinValue, uint16_t rawMaxValue);
    virtual double read() = 0;
    void setRange(double minValue, double maxValue);
    void setConversion(uint16_t rawMinValue, uint16_t rawMaxValue);
    double convertedValue();
    double validatedConvertedValue();
    bool isInRange();

protected:
    uint16_t rawValue;
    double maxValue, minValue;
    uint16_t rawMaxValue, rawMinValue;
    double intercept, slope;
};

class Apps : public Sensor
{
public:
    Apps(double minValue, double maxValue, uint16_t rawMinValue, uint16_t rawMaxValue, uint8_t pin);
    double read();

protected:
    uint8_t pin;
};
class Tps : public Sensor
{
public:
    Tps(double minValue, double maxValue, uint16_t rawMinValue, uint16_t rawMaxValue, uint8_t pin);
    double read();

protected:
    uint8_t pin;
};

class Bse : Sensor
{
public:
    double read();
};

#endif