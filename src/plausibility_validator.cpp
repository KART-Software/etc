#include "plausibility_validator.hpp"

//TODO リファクタ

PlausibilityValidator::PlausibilityValidator(
    double *apps1,
    double *apps2,
    double *tps1,
    double *tps2) : apps1(apps1), apps2(apps2), tps1(tps1), tps2(tps2)
{
    lastAppsPlausibleTime = 0;
    lastTpsPlausibleTime = 0;
    lastApps1CircuitValidTime = 0;
    lastTps1CircuitValidTime = 0;
}

bool PlausibilityValidator::isAppsPlausible()
{
    unsigned long now = millis();
    if (abs(*apps1 - *apps2) < 10.0)
    { //TODO
        lastAppsPlausibleTime = now;
        return true;
    }
    if (now - lastAppsPlausibleTime > SENSOR_IMPLAUSIBLE_THRESHOLD_TIME)
    {
        gErrorHandler.raise(ERR_APPS_IMPLAUSIBLE);
        return false;
    }
    return true;
}

bool PlausibilityValidator::isTpsPlausible()
{
    unsigned long now = millis();
    if (abs(*tps1 - *tps2) < 10.0)
    { //TODO
        lastTpsPlausibleTime = now;
        return true;
    }
    if (now - lastTpsPlausibleTime > SENSOR_IMPLAUSIBLE_THRESHOLD_TIME)
    {
        gErrorHandler.raise(ERR_TPS_IMPLAUSIBLE);
        return false;
    }
    return true;
}

bool PlausibilityValidator::isApps1CircuitValid()
{
    unsigned long now = millis();
    if (*apps1 > APPS_1_MIN && *apps1 < APPS_1_MAX)
    { //TODO
        lastApps1CircuitValidTime = now;
        return true;
    }
    if (now - lastAppsPlausibleTime > SENSOR_IMPLAUSIBLE_THRESHOLD_TIME)
    {
        gErrorHandler.raise(ERR_APPS_1_CIRCUIT_FAILURE);
        return false;
    }
    return true;
}

bool PlausibilityValidator::isApps2CircuitValid()
{
    unsigned long now = millis();
    if (*apps2 > APPS_2_MIN && *apps2 < APPS_2_MAX)
    { //TODO
        lastApps2CircuitValidTime = now;
        return true;
    }
    if (now - lastAppsPlausibleTime > SENSOR_IMPLAUSIBLE_THRESHOLD_TIME)
    {
        gErrorHandler.raise(ERR_APPS_2_CIRCUIT_FAILURE);
        return false;
    }
    return true;
}
