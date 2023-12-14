#ifndef _PLAUSIBILITY_VALIDATOR_H_
#define _PLAUSIBILITY_VALIDATOR_H_

#include <Arduino.h>
#include "sensors.hpp"
#include "error_handler.hpp"
#include "globals.hpp"

#define SENSOR_IMPLAUSIBLE_THRESHOLD_TIME 100
#define APPS_TPS_TARGET_IMPLAUSIBLE_THRESHOLD_TIME 1000
#define BPS_TPS_IMPLAUSIBLE_THRESHOLD_TIME 1000
#define SENSOR_SAME_POSITION_THRESHOLD 10.0
#define PLAUSIBLE_DURATION 500

#define SERIAL_SPEED 115200
#define SERIAL_LOG_INTERVAL 150

class PlausibilityValidator
{
public:
    PlausibilityValidator(Apps &apps1, Apps &apps2, Tps &tps1, Tps &tps2, Bps &bps);
    PlausibilityValidator(Apps &apps1, Apps &apps2, Tps &tps1, Tps &tps2, Apps &targetSensor, Bps &bps);
    void initialize();
    bool isCurrentlyValid();
    bool isValid();
    void serialLog();
    void startLog();
    void setCheckFlags(bool apps, bool tps, bool apps1, bool apps2, bool tps1, bool tps2, bool target, bool bps, bool bpsTps);
    bool appsCheckFlag = false,
         tpsCheckFlag = false,
         apps1CheckFlag = false,
         apps2CheckFlag = false,
         tps1CheckFlag = false,
         tps2CheckFlag = false,
         targetCheckFlag = false,
         bpsCheckFlag = false,
         bpsTpsCheckFlag = false;

private:
    ErrorHandler errorHandler = ErrorHandler();
    Apps &apps1, &apps2;
    Tps &tps1, &tps2;
    Apps &targetSensor;
    Bps &bps;
    bool isValidAllTime;
    unsigned long
        lastTpsPlausibleTime,
        lastAppsPlausibleTime,
        lastTps1CircuitValidTime,
        lastTps2CircuitValidTime,
        lastApps1CircuitValidTime,
        lastApps2CircuitValidTime,
        lastAppsTpsTargetValidTime,
        lastBpsCircuitValidTime,
        lastBpsTpsPlausibleTime;

    void initParameters();
    bool isAppsPlausible();
    bool isTpsPlausible();
    bool isApps1CircuitValid();
    bool isApps2CircuitValid();
    bool isTps1CircuitValid();
    bool isTps2CircuitValid();
    bool isAppsTpsTargetValid();
    bool isBpsCircuitValid();
    bool isBpsTpsPlausible();
    bool hasIttr = false;
    String toNChars(String value, uint8_t n);
};

void startLogging(void *plausibilityValidator);

#endif