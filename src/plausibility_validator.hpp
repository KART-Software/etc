#ifndef _PLAUSIBILITY_VALIDATOR_H_
#define _PLAUSIBILITY_VALIDATOR_H_

#include <Arduino.h>
#include "sensors.hpp"
#include "error_handler.hpp"
#include "globals.hpp"

#define SENSOR_IMPLAUSIBLE_THRESHOLD_TIME 100
#define SENSOR_SAME_POSITION_THRESHOLD 10.0

#define SERIAL_SPEED 115200
#define SERIAL_LOG_INTERVAL 150

class PlausibilityValidator
{
public:
    PlausibilityValidator(Apps &apps1, Apps &apps2, Tps &tps1, Tps &tps2);
    PlausibilityValidator(Apps &apps1, Apps &apps2, Tps &tps1, Tps &tps2, Apps &targetSensor);
    void initialize();
    bool isCurrentlyValid();
    bool isValid();
    void serialLog();
    void startLog();

private:
    ErrorHandler errorHandler = ErrorHandler();
    Apps &apps1, &apps2;
    Tps &tps1, &tps2;
    Apps &targetSensor;
    bool isValidAllTime;
    unsigned long
        lastTpsPlausibleTime,
        lastAppsPlausibleTime,
        lastTps1CircuitValidTime,
        lastTps2CircuitValidTime,
        lastApps1CircuitValidTime,
        lastApps2CircuitValidTime,
        lastAppsTpsTargetValidTime;
    void initParameters();
    bool isAppsPlausible();
    bool isTpsPlausible();
    bool isApps1CircuitValid();
    bool isApps2CircuitValid();
    bool isTps1CircuitValid();
    bool isTps2CircuitValid();
    bool isAppsTpsTargetValid();
    bool hasIttr = false;
    String toNChars(String value, uint8_t n);
};

void startLogging(void *plausibilityValidator);

#endif