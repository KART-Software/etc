#ifndef _SENSORS_H_
#define _SENSORS_H_

#include <Arduino.h>

#include "constants.hpp"
#include "globals.hpp"

class Sensor
{
public:
    Sensor(uint16_t rawMinValue, uint16_t rawMaxValue, double minValue, double maxValue, double margin);
    virtual double read() = 0;
    void setRange(double minValue, double maxValue);
    void setConversion(uint16_t rawMinValue, uint16_t rawMaxValue);
    double convertedValue();
    double validatedConvertedValue();
    bool isInRange();
    double getMaxValue();
    double getMinValue();
    uint16_t getRawValue();

protected:
    uint16_t rawValue;
    double maxValue, minValue;
    double margin;
    double intercept, slope;
};

class Apps : public Sensor
{
public:
    Apps(uint16_t rawMinValue, uint16_t rawMaxValue, uint8_t ch, double minValue = APPS_MIN, double maxValue = APPS_MAX, double margin = APPS_MARGIN);
    double read();
    double convertToTargetTp();

private:
    uint8_t ch;
};

class Tps : public Sensor
{
public:
    Tps(uint16_t rawMinValue, uint16_t rawMaxValue, uint8_t ch, double minValue = TPS_MIN, double maxValue = TPS_MAX, double margin = TPS_MARGIN);
    double read();

private:
    uint8_t ch;
};

class Ittr : public Apps
{
public:
    Ittr(uint16_t rawMinValue = ITTR_RAW_MIN, uint16_t rawMaxValue = ITTR_RAW_MAX, uint8_t ch = ITTR_CH, double minValue = ITTR_MIN, double maxValue = ITTR_MAX, double margin = ITTR_MARGIN);
};

class Bps : public Sensor
{
public:
    Bps(uint16_t rawMinValue = BPS_RAW_MIN, uint16_t rawMaxValue = BPS_RAW_MAX, uint8_t ch = BPS_CH, double minValue = BPS_MIN, double maxValue = BPS_MAX, double highPressureThreshold = BPS_HIGH_PRESSURE_THRESHOLD, double margin = BPS_MARGIN);
    double read();
    bool isHighPressure();

private:
    uint8_t ch;

    double highPressureThreshold;
};

#endif