#ifndef _SENSORS_H_
#define _SENSORS_H_

#include <Arduino.h>

#include "constants.hpp"
#include "globals.hpp"
#include "moving_average.hpp"

class Sensor
{
public:
    Sensor(uint16_t rawMinValue, uint16_t rawMaxValue, double minValue, double maxValue, double margin);
    virtual void read() = 0;
    void setConversion(double minValue, double maxValue);
    void setConversion();
    void setRawMin(uint16_t val);
    void setRawMax(uint16_t val);
    uint16_t setCurrentValRawMin();
    uint16_t setCurrentValRawMax();
    double convertedValue();
    bool isInRange();
    double getMaxValue();
    double getMinValue();
    uint16_t getRawValue();

protected:
    MovingAverage mvgAvg = MovingAverage(60);
    uint16_t rawValue;
    uint16_t rawMinValue, rawMaxValue;
    const double maxValue, minValue;
    const double margin;
    double intercept, slope;
};

class Apps : public Sensor
{
public:
    Apps(uint16_t rawMinValue, uint16_t rawMaxValue, uint8_t ch, double minValue = APPS_MIN, double maxValue = APPS_MAX, double margin = APPS_MARGIN, double idlingValue = APPS_IDLING);
    void read();
    double convertToTargetTp();
    void setIdlingValue(double val);
    void setIdling(bool idling);

private:
    const uint8_t ch;
    double idlingValue;
    bool idling = true;
};

class Tps : public Sensor
{
public:
    Tps(uint16_t rawMinValue, uint16_t rawMaxValue, uint8_t ch, double minValue = TPS_MIN, double maxValue = TPS_MAX, double largeOpenThreshold = TPS_LARGE_OPEN_THRESHOLD, double margin = TPS_MARGIN);
    void read();
    bool isLargeOpen();

private:
    const uint8_t ch;
    const double largeOpenThreshold;
};

class Ittr : public Apps
{
public:
    Ittr(uint16_t rawMinValue = ITTR_RAW_MIN, uint16_t rawMaxValue = ITTR_RAW_MAX, uint8_t ch = ITTR_CH, double minValue = ITTR_MIN, double maxValue = ITTR_MAX, double margin = ITTR_MARGIN, double idlingValue = ITTR_IDLING);
};

class Bps : public Sensor
{
public:
    Bps(uint16_t rawMinValue = BPS_RAW_MIN, uint16_t rawMaxValue = BPS_RAW_MAX, uint8_t ch = BPS_CH, double minValue = BPS_MIN, double maxValue = BPS_MAX, double highPressureThreshold = BPS_HIGH_PRESSURE_THRESHOLD, double margin = BPS_MARGIN);
    void read();
    bool isHighPressure();

private:
    const uint8_t ch;
    const double highPressureThreshold;
};

#endif