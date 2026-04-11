#ifndef _CONFIG_MODEL_H_
#define _CONFIG_MODEL_H_

#include <ArduinoJson.h>
#include "constants.hpp"

#define CONFIG_FILE_NAME "/config.json"
#define CONFIG_JSON_SIZE 768

struct SensorValues
{
    uint16_t apps1Min, apps1Max, apps2Min, apps2Max, ittrMin, ittrMax, tps1Min, tps1Max, tps2Min, tps2Max;
    double idling;
};

struct PlausibilityCheckFlags
{
    bool apps, tps, apps1, apps2, tps1, tps2, target, bps, bpsTps;
};

struct PidGains
{
    double kP, kI, kD;
};

struct ConfigModel
{
    SensorValues sensorValues;
    PlausibilityCheckFlags plausibilityFlags;
    bool useIttr;
    PidGains pid;

    void loadFromConstants();
    bool loadFromJson(const String &jsonStr);
    void toJson(JsonObject &out) const;
};

#endif
