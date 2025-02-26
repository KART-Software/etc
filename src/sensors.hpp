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
    const double minValue, maxValue;
    const double margin;
    double intercept, slope;
};

class Apps : public Sensor
{
public:
    Apps(uint16_t rawMinValue, uint16_t rawMaxValue, uint8_t ch, double minValue = APPS_MIN, double maxValue = APPS_MAX, double margin = APPS_MARGIN);
    void read();
    double constrainedValue();

private:
    const uint8_t ch;
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
    Ittr(uint16_t rawMinValue = ITTR_RAW_MIN, uint16_t rawMaxValue = ITTR_RAW_MAX, uint8_t ch = ITTR_CH, double minValue = APPS_MIN, double maxValue = APPS_MAX, double margin = ITTR_MARGIN);
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

class Target
{
public:
    Target(Apps &apps, Ittr &ittr);
    double getTarget();
    void setModeCalibration();
    void setModeNormal();
    void setModeRestricted();
    bool isIttr();
    void setIttr(bool isIttr);
    void setIdlingValue(double val);
    const char *getModeString();
    // void setNormalMaxValue(double val);
    // void setRestrictedMaxValue(double val);

    void read();
    uint16_t getSensorRawValue();
    double getSensorValue();

    bool setManual();
    bool isManual();
    double manualPlus();
    double manualMinus();

private:
    enum class Mode
    {
        Calibration,
        Normal,
        Restricted
    };
    Mode mode;
    Apps &apps;
    Ittr &ittr;
    bool _isIttr;
    double idlingValue;
    double minValue, maxValue;
    double manualTarget;
    bool _isManual = false;

    const double normalMaxValue = TARGET_NORMAL_MAX;
    const double restrictedMaxValue = TARGET_RESTRICTED_MAX;
    double tpsMinValue = TPS_MIN;
    double tpsMaxValue = TPS_MAX;
};
#endif