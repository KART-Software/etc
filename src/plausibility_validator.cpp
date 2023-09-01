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
    bool flag = true;
    // flag *= isAppsPlausible();
    flag &= isTpsPlausible();
    // flag *= isApps1CircuitValid();
    // flag *= isApps2CircuitValid();
    flag &= isTps1CircuitValid();
    flag &= isTps2CircuitValid();
    // flag *= isAppsTpsTargetValid();
    // flag *= isBpsCircuitValid();
    // flag *= isBpsTpsPlausible();
    flag |= millis() < PLAUSIBLE_DURATION;

    isValidAllTime &= flag;

    return flag;
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
    String logStr;
    if (hasIttr)
    {
        logStr = "APPS1: " + toNChars(String(apps1.getRawValue()), 5) +
                 ", APPS2: " + toNChars(String(apps2.getRawValue()), 5) +
                 ", ITTR: " + toNChars(String(targetSensor.getRawValue()), 5) +
                 ", TPS1: " + toNChars(String(tps1.getRawValue()), 5) +
                 ", TPS2: " + toNChars(String(tps2.getRawValue()), 5) +
                 ", BPS: " + toNChars(String(bps.getRawValue()), 5) +
                 ",   APPS1: " + toNChars(String(apps1.convertedValue()), 7) +
                 ", APPS2: " + toNChars(String(apps2.convertedValue()), 7) +
                 ", ITTR: " + toNChars(String(targetSensor.convertedValue()), 7) +
                 ", TPS1: " + toNChars(String(tps1.convertedValue()), 7) +
                 ", TPS2: " + toNChars(String(tps2.convertedValue()), 7) +
                 ", BPS: " + toNChars(String(bps.convertedValue()), 7);
    }
    else
    {
        logStr = "APPS1: " + toNChars(String(apps1.getRawValue()), 5) +
                 ", APPS2: " + toNChars(String(apps2.getRawValue()), 5) +
                 ", TPS1: " + toNChars(String(tps1.getRawValue()), 5) +
                 ", TPS2: " + toNChars(String(tps2.getRawValue()), 5) +
                 ", BPS: " + toNChars(String(bps.getRawValue()), 5) +
                 ",   APPS1: " + toNChars(String(apps1.convertedValue()), 7) +
                 ", APPS2: " + toNChars(String(apps2.convertedValue()), 7) +
                 ", TPS1: " + toNChars(String(tps1.convertedValue()), 7) +
                 ", TPS2: " + toNChars(String(tps2.convertedValue()), 7) +
                 ", BPS: " + toNChars(String(bps.convertedValue()), 7);
    }
    Serial.println(logStr);
}

void PlausibilityValidator::startLog()
{
    while (true)
    {
        serialLog();
        delay(SERIAL_LOG_INTERVAL);
    }
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