#include "plausibility_validator.hpp"

PlausibilityValidator::PlausibilityValidator(Apps &apps1, Apps &apps2, Tps &tps1, Tps &tps2, Bps &bps)
    : apps1(apps1), apps2(apps2), tps1(tps1), tps2(tps2), targetSensor(apps1), bps(bps)
{
    initParameters();
}

PlausibilityValidator::PlausibilityValidator(Apps &apps1, Apps &apps2, Tps &tps1, Tps &tps2, Apps &targetSensor, Bps &bps)
    : apps1(apps1), apps2(apps2), tps1(tps1), tps2(tps2), targetSensor(targetSensor), bps(bps)
{
    initParameters();
    hasIttr = true;
}

void PlausibilityValidator::initialize()
{
    Serial.begin(SERIAL_SPEED);
}

void PlausibilityValidator::initParameters()
{
    lastAppsPlausibleTime = 0;
    lastTpsPlausibleTime = 0;
    lastTps1CircuitValidTime = 0;
    lastTps2CircuitValidTime = 0;
    lastApps1CircuitValidTime = 0;
    lastApps2CircuitValidTime = 0;
    lastAppsTpsTargetValidTime = 0;
    lastBpsCircuitValidTime = 0;
    lastBpsTpsPlausibleTime = 0;
    isValidAllTime = true;
}

bool PlausibilityValidator::isCurrentlyValid()
{
    bool isValid = true;
    isValid &= isAppsPlausible() || !appsCheckFlag;
    isValid &= isTpsPlausible() || !tpsCheckFlag;
    isValid &= isApps1CircuitValid() || !apps1CheckFlag;
    isValid &= isApps2CircuitValid() || !apps2CheckFlag;
    isValid &= isTps1CircuitValid() || !tps1CheckFlag;
    isValid &= isTps2CircuitValid() || !tps2CheckFlag;
    isValid &= isAppsTpsTargetValid() || !targetCheckFlag;
    isValid &= isBpsCircuitValid() || !bpsCheckFlag;
    isValid &= isBpsTpsPlausible() || !bpsTpsCheckFlag;
    isValid |= millis() < PLAUSIBLE_DURATION;

    isValidAllTime &= isValid;

    return isValid;
}

bool PlausibilityValidator::isValid()
{
    isValidAllTime &= isCurrentlyValid();
    return isValidAllTime;
}

bool PlausibilityValidator::isAppsPlausible()
{
    unsigned long now = millis();
    if (abs(apps1.convertedValue() - apps2.convertedValue()) < SENSOR_SAME_POSITION_THRESHOLD)
    {
        lastAppsPlausibleTime = now;
        return true;
    }
    if (now - lastAppsPlausibleTime > SENSOR_IMPLAUSIBLE_THRESHOLD_TIME)
    {
        errorHandler.raise(ERR_APPS_IMPLAUSIBLE);
        return false;
    }
    return true;
}

bool PlausibilityValidator::isTpsPlausible()
{
    unsigned long now = millis();
    if (abs(tps1.convertedValue() - tps2.convertedValue()) < SENSOR_SAME_POSITION_THRESHOLD)
    {
        lastTpsPlausibleTime = now;
        return true;
    }
    if (now - lastTpsPlausibleTime > SENSOR_IMPLAUSIBLE_THRESHOLD_TIME)
    {
        errorHandler.raise(ERR_TPS_IMPLAUSIBLE);
        return false;
    }
    return true;
}

bool PlausibilityValidator::isApps1CircuitValid()
{
    unsigned long now = millis();
    if (apps1.isInRange())
    {
        lastApps1CircuitValidTime = now;
        return true;
    }
    if (now - lastApps1CircuitValidTime > SENSOR_IMPLAUSIBLE_THRESHOLD_TIME)
    {
        errorHandler.raise(ERR_APPS_1_CIRCUIT_FAILURE);
        return false;
    }
    return true;
}

bool PlausibilityValidator::isApps2CircuitValid()
{
    unsigned long now = millis();
    if (apps2.isInRange())
    {
        lastApps2CircuitValidTime = now;
        return true;
    }
    if (now - lastApps2CircuitValidTime > SENSOR_IMPLAUSIBLE_THRESHOLD_TIME)
    {
        errorHandler.raise(ERR_APPS_2_CIRCUIT_FAILURE);
        return false;
    }
    return true;
}

bool PlausibilityValidator::isTps1CircuitValid()
{
    unsigned long now = millis();
    if (tps1.isInRange())
    {
        lastTps1CircuitValidTime = now;
        return true;
    }
    if (now - lastTps1CircuitValidTime > SENSOR_IMPLAUSIBLE_THRESHOLD_TIME)
    {
        errorHandler.raise(ERR_TPS_1_CIRCUIT_FAILURE);
        return false;
    }
    return true;
}

bool PlausibilityValidator::isTps2CircuitValid()
{
    unsigned long now = millis();
    if (tps2.isInRange())
    {
        lastTps2CircuitValidTime = now;
        return true;
    }
    if (now - lastTps2CircuitValidTime > SENSOR_IMPLAUSIBLE_THRESHOLD_TIME)
    {
        errorHandler.raise(ERR_TPS_2_CIRCUIT_FAILURE);
        return false;
    }
    return true;
}

bool PlausibilityValidator::isAppsTpsTargetValid()
{
    unsigned long now = millis();
    double targetTp = targetSensor.convertToTargetTp();
    double tpsValue = tps1.convertedValue();
    if (abs(targetTp - tpsValue) < SENSOR_SAME_POSITION_THRESHOLD)
    {
        lastAppsTpsTargetValidTime = now;
        return true;
    }
    if (now - lastAppsTpsTargetValidTime > APPS_TPS_TARGET_IMPLAUSIBLE_THRESHOLD_TIME)
    {
        errorHandler.raise(ERR_APPS_TPS_TARGET_FAILURE);
        return false;
    }
    return true;
}

bool PlausibilityValidator::isBpsCircuitValid()
{
    unsigned long now = millis();
    if (bps.isInRange())
    {
        lastBpsCircuitValidTime = now;
        return true;
    }
    if (now - lastBpsCircuitValidTime > SENSOR_IMPLAUSIBLE_THRESHOLD_TIME)
    {
        errorHandler.raise(ERR_BPS_CIRCUIT_FAILURE);
        return false;
    }
    return true;
}

bool PlausibilityValidator::isBpsTpsPlausible()
{
    unsigned long now = millis();
    double targetTp = targetSensor.convertToTargetTp();
    double tpsValue = tps1.convertedValue();
    if (!bps.isHighPressure() || tps1.isLargeOpen())
    {
        lastBpsTpsPlausibleTime = now;
        return true;
    }
    if (now - lastBpsTpsPlausibleTime > BPS_TPS_IMPLAUSIBLE_THRESHOLD_TIME)
    {
        errorHandler.raise(ERR_BPS_TPS_IMPLAUSIBLE);
        return false;
    }
    return true;
}

void PlausibilityValidator::serialLog()
{
    if (hasIttr)
    {
        Serial.printf("APPS1: %5.2d, APPS2: %5.2d, ITTR: %5.2d, TPS1: %5.2d, TPS2: %5.2d, BPS: %5.2d, APPS1: %7.2lf, APPS2: %7.2lf, ITTR: %7.2lf, TPS1: %7.2lf, TPS2: %7.2lf, BPS: %8.2lf\n",
                      apps1.getRawValue(),
                      apps2.getRawValue(),
                      targetSensor.getRawValue(),
                      tps1.getRawValue(),
                      tps2.getRawValue(),
                      bps.getRawValue(),
                      apps1.convertedValue(),
                      apps2.convertedValue(),
                      targetSensor.convertToTargetTp(),
                      tps1.convertedValue(),
                      tps2.convertedValue(),
                      bps.convertedValue());
    }
    else
    {
        Serial.printf("APPS1: %5.2d, APPS2: %5.2d, TPS1: %5.2d, TPS2: %5.2d, BPS: %5.2d, APPS1: %7.2lf, APPS2: %7.2lf, TARGET: %7.2lf, TPS1: %7.2lf, TPS2: %7.2lf, BPS: %8.2lf\n",
                      apps1.getRawValue(),
                      apps2.getRawValue(),
                      tps1.getRawValue(),
                      tps2.getRawValue(),
                      bps.getRawValue(),
                      apps1.convertedValue(),
                      apps2.convertedValue(),
                      targetSensor.convertToTargetTp(),
                      tps1.convertedValue(),
                      tps2.convertedValue(),
                      bps.convertedValue());
    }
}

void PlausibilityValidator::startLog()
{
    while (true)
    {
        serialLog();
        delay(SERIAL_LOG_INTERVAL);
    }
}

void PlausibilityValidator::setCheckFlags(bool apps, bool tps, bool apps1, bool apps2, bool tps1, bool tps2, bool target, bool bps, bool bpsTps)
{
    appsCheckFlag = apps;
    tpsCheckFlag = tps;
    apps1CheckFlag = apps1;
    apps2CheckFlag = apps2;
    tps1CheckFlag = tps1;
    tps2CheckFlag = tps2;
    targetCheckFlag = target;
    bpsCheckFlag = bps;
    bpsTpsCheckFlag = bpsTps;
}

String PlausibilityValidator::toNChars(String value, uint8_t n)
{
    int spaces = n - value.length();
    for (int i = 0; i < spaces; i++)
    {
        value = " " + value;
    }
    return value;
}

void startLogging(void *plausibilityValidator)
{
    PlausibilityValidator *validator;
    validator = (PlausibilityValidator *)plausibilityValidator;
    validator->startLog();
}