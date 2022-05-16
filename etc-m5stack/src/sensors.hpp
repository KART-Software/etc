#ifndef _SENSORS_H_
#define _SENSORS_H_

#include <M5Stack.h>

#include "init_pins.hpp"
#include "globals.hpp"

#define TPS_1_RAW_MAX 537
#define TPS_2_RAW_MAX 481

#define TPS_1_RAW_MIN 359
#define TPS_2_RAW_MIN 659

#define TPS_1_MAX 100
#define TPS_2_MAX 100

#define TPS_1_MIN 0
#define TPS_2_MIN 0

#define TPS_MARGIN 5

#define TP_SAME_POSITION_THRESHOLD 2

#define APPS_1_RAW_MAX 1
#define APPS_2_RAW_MAX 1023

#define APPS_1_RAW_MIN 1023
#define APPS_2_RAW_MIN 1

#define APPS_1_MAX 100
#define APPS_2_MAX 100

#define APPS_1_MIN 0
#define APPS_2_MIN 0

#define APPS_MARGIN 50

class Sensor
{
public:
    Sensor();
    Sensor(double minValue, double maxValue, double margin, int16_t rawMinValue, int16_t rawMaxValue);
    virtual double read() = 0;
    void setRange(double minValue, double maxValue);
    void setConversion(int16_t rawMinValue, int16_t rawMaxValue);
    double convertedValue();
    double validatedConvertedValue();
    bool isInRange();
    double getMaxValue();
    double getMinValue();

protected:
    int16_t rawValue;
    double maxValue, minValue;
    double margin;
    double intercept, slope;
};

class Apps : public Sensor
{
public:
    Apps(double minValue, double maxValue, double margin, int16_t rawMinValue, int16_t rawMaxValue, uint8_t pin);
    double read();
    double convertToTargetTp();

protected:
    uint8_t pin;
};
class Tps : public Sensor
{
public:
    Tps(double minValue, double maxValue, double margin, int16_t rawMinValue, int16_t rawMaxValue, uint8_t pin);
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