#ifndef _SENSORS_H_
#define _SENSORS_H_

#include <Arduino.h>

#include "constants.hpp"
#include "globals.hpp"

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
    int16_t getRawValue();

protected:
    int16_t rawValue;
    double maxValue, minValue;
    double margin;
    double intercept, slope;
};

class Apps : public Sensor
{
public:
    Apps(double minValue, double maxValue, double margin, int16_t rawMinValue, int16_t rawMaxValue, uint8_t ch);
    double read();
    double convertToTargetTp();

protected:
    uint8_t ch;
};

class Tps : public Sensor
{
public:
    Tps(double minValue, double maxValue, double margin, int16_t rawMinValue, int16_t rawMaxValue, uint8_t ch);
    double read();

protected:
    uint8_t ch;
};

class Ittr : public Apps
{
public:
    Ittr(double minValue, double maxValue, double margin, int16_t rawMinValue, int16_t rawMaxValue, uint8_t ch);
};

class Bse : Sensor
{
public:
    double read();
};

#endif