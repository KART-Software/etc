#ifndef _PLAUSIBILITY_VALIDATOR_H_
#define _PLAUSIBILITY_VALIDATOR_H_

#include <Arduino.h>
#include "sensors.hpp"
#include "error_handler.hpp"
#include "globals.hpp"

#define SENSOR_IMPLAUSIBLE_THRESHOLD_TIME 100
#define SENSOR_SAME_POSITION_THRESHOLD 10.0

class PlausibilityValidator
{
public:
    PlausibilityValidator(Apps &apps1, Apps &apps2, Tps &tps1, Tps &tps2);
    PlausibilityValidator(Apps &apps1, Apps &apps2, Tps &tps1, Tps &tps2, Apps &targetSensor);
    bool isCurrentlyValid();
    bool isValid();

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
    void initialize();
    bool isAppsPlausible();
    bool isTpsPlausible();
    bool isApps1CircuitValid();
    bool isApps2CircuitValid();
    bool isTps1CircuitValid();
    bool isTps2CircuitValid();
    bool isAppsTpsTargetValid();
};

#endif