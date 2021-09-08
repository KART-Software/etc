#ifndef _SENSORS_H_
#define _SENSORS_H_

#include <M5Stack.h>

#include "init_pins.hpp"
#include "globals.hpp"

#define TPS_1_RAW_MAX 100
#define TPS_2_RAW_MAX 100

#define TPS_1_RAW_MIN 400
#define TPS_2_RAW_MIN 400

#define TPS_1_DIRECTION -1
#define TPS_2_DIRECTION -1

#define TPS_1_MAX 100
#define TPS_2_MAX 100

#define TPS_1_MIN 0
#define TPS_2_MIN 0

#define APPS_MARGIN 20
#define APPS1_DIRECTION -1
#define APPS2_DIRECTION 1

#define APPS_1_RAW_MAX 20
#define APPS_2_RAW_MAX 1000

#define APPS_1_RAW_MIN 1000
#define APPS_2_RAW_MIN 20

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