#ifndef _PLAUSIBILITY_VALIDATOR_H_
#define _PLAUSIBILITY_VALIDATOR_H_

#include <Arduino.h>
#include "sensors.hpp"
#include "error_handler.hpp"
#include "globals.hpp"

#define SENSOR_IMPLAUSIBLE_THRESHOLD_TIME 100

class PlausibilityValidator
{
public:
    PlausibilityValidator(double *apps1, double *apps2, double *tps1, double *tps2);
    bool isValid();

private:
    double *apps1, *apps2, *tps1, *tps2;
    unsigned long
        lastTpsPlausibleTime,
        lastAppsPlausibleTime,
        lastTps1CircuitValidTime,
        lastTps2CircuitValidTime,
        lastApps1CircuitValidTime,
        lastApps2CircuitValidTime;
    bool isAppsPlausible();
    bool isTpsPlausible();
    bool isApps1CircuitValid();
    bool isApps2CircuitValid();
    void checkTpsAppsPlausibility();
    void checkAppsPlausibility();
    void checkTpsPlausibility();
    void checkTpsCircuit();
};

#endif