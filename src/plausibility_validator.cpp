#include "plausibility_validator.hpp"

PlausibilityValidator::PlausibilityValidator(Apps &apps1, Apps &apps2, Ittr &ittr, Tps &tps1, Tps &tps2, Target &target, Bps &bps)
    : apps1(apps1), apps2(apps2), ittr(ittr), tps1(tps1), tps2(tps2), target(target), bps(bps)
{
}

void PlausibilityValidator::initialize()
{
    Serial.begin(SERIAL_SPEED);
    initParameters();
}

void PlausibilityValidator::initParameters()
{
    unsigned long now = millis();
    lastAppsPlausibleTime = now;
    lastTpsPlausibleTime = now;
    lastTps1CircuitValidTime = now;
    lastTps2CircuitValidTime = now;
    lastApps1CircuitValidTime = now;
    lastApps2CircuitValidTime = now;
    lastAppsTpsTargetValidTime = now;
    lastBpsCircuitValidTime = now;
    lastBpsTpsPlausibleTime = now;
    isValidAllTime = true;
}

bool PlausibilityValidator::isCurrentlyValid()
{
    if (millis() < PLAUSIBLE_DURATION)
    {
        isValidAllTime = true;
        return true;
    }
    else
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

        isValidAllTime &= isValid;

        return isValid;
    }
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
    double targetTp = target.getTarget();
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
    if (!bps.isHighPressure() || !tps1.isLargeOpen())
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
    Serial.printf("%s %s %sAPPS\e[m: %5.2d %5.2d %s%7.2lf%%\e[m %s%7.2lf%%\e[m, ITTR: %5.2d %7.2lf%%, %sTARGET\e[m:%s: %7.2lf%% %sTPS\e[m: %5.2d %5.2d %s%7.2lf%%\e[m %s%7.2lf%%\e[m, %sBPS\e[m: %5.2d %s%7.2lfpsi\e[m\r",
                  target.getModeString(),
                  isValidAllTime ? OK : ERR,
                  color(appsCheckFlag, errorHandler.raised(ERR_APPS_IMPLAUSIBLE)),
                  apps1.getRawValue(),
                  apps2.getRawValue(),
                  color(apps1CheckFlag, errorHandler.raised(ERR_APPS_1_CIRCUIT_FAILURE)),
                  apps1.convertedValue(),
                  color(apps2CheckFlag, errorHandler.raised(ERR_APPS_2_CIRCUIT_FAILURE)),
                  apps2.convertedValue(),
                  ittr.getRawValue(),
                  ittr.convertedValue(),
                  color(targetCheckFlag, errorHandler.raised(ERR_APPS_TPS_TARGET_FAILURE)),
                  target.isManual() ? "M" : (target.isIttr() ? "I" : "A"),
                  target.getTarget(),
                  color(tpsCheckFlag, errorHandler.raised(ERR_TPS_IMPLAUSIBLE)),
                  tps1.getRawValue(),
                  tps2.getRawValue(),
                  color(tps1CheckFlag, errorHandler.raised(ERR_TPS_1_CIRCUIT_FAILURE)),
                  tps1.convertedValue(),
                  color(tps2CheckFlag, errorHandler.raised(ERR_TPS_2_CIRCUIT_FAILURE)),
                  tps2.convertedValue(),
                  color(bpsTpsCheckFlag, errorHandler.raised(ERR_BPS_TPS_IMPLAUSIBLE)),
                  bps.getRawValue(),
                  color(bpsCheckFlag, errorHandler.raised(ERR_BPS_CIRCUIT_FAILURE)),
                  bps.convertedValue());
}

const char *PlausibilityValidator::color(bool flag, bool err)
{
    if (flag && err)
    {
        return RED;
    }
    else if (flag && !err)
    {
        return GRN;
    }
    else if (!flag && err)
    {
        return RED_;
    }
    else
    {
        return GRN_;
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

void startLogging(void *plausibilityValidator)
{
    PlausibilityValidator *validator;
    validator = (PlausibilityValidator *)plausibilityValidator;
    validator->startLog();
}