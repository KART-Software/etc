#ifndef _CONFIGURATOR_H_
#define _CONFIGURATOR_H_

#include "sensors.hpp"
#include "motor_controller.hpp"
#include <ArduinoJson.h>
#include "flash.hpp"
#include "plausibility_validator.hpp"
#include "serial_protocol.hpp"

#define SENSOR_VALUES_FILE_NAME "/sensor_values.txt"
#define PLAUSIBILITY_CHECK_FLAGS_FILE_NAME "/plausibility_check_flags.txt"
#define USE_ITTR_FLAG_FILE_NAME "/use_ittr_flag.txt"
#define RAW_SENSOR_VALUES_JSON_SIZE 300
#define PLAUSIBILITY_CHECK_FLAGS_JSON_SIZE 300
#define USE_ITTR_FLAG_JSON_SIZE 50

struct Config
{
public:
    virtual bool loadFromJsonStr(const char *jsonStr);
    virtual void loadFromConstants();
    virtual String serialize();
};

struct RawSensorValues : Config
{
public:
    uint16_t apps1Min, apps1Max, apps2Min, apps2Max, ittrMin, ittrMax, tps1Min, tps1Max, tps2Min, tps2Max;
    double idling;
    bool loadFromJsonStr(const char *jsonStr);
    void loadFromConstants();
    String serialize();
};

struct PlausibilityCheckFlags : Config
{
public:
    bool apps, tps, apps1, apps2, tps1, tps2, target, bps, bpsTps;
    bool loadFromJsonStr(const char *jsonStr);
    void loadFromConstants();
    String serialize();
};

struct UseIttrFlag : Config
{
public:
    bool useIttr;
    bool loadFromJsonStr(const char *jsonStr);
    void loadFromConstants();
    String serialize();
};

class Configurator
{
public:
    Configurator(Apps &apps1, Apps &apps2, Tps &tps1, Tps &tps2, Ittr &ittr, Target &target, MotorController &motorController, PlausibilityValidator &plausibilityValidator);
    void initialize();
    void calibrateFromFlash();
    void getConfigJson(JsonObject &out);

    void startCalibration();
    void finishCalibration();
    void setAppsMin();
    void setAppsMax();
    void setTpsMin();
    void setTpsMax();
    void setIdling();
    bool setPlausibilityFlag(const char *key, bool val);
    void setIttrFlag(bool val);
    bool importConfig(const char *jsonStr);

private:
    Flash flash;
    RawSensorValues rawValues;
    PlausibilityCheckFlags plausibilityCheckFlags;
    UseIttrFlag useIttrFlag;
    Apps &apps1, &apps2;
    Tps &tps1, &tps2;
    Ittr &ittr;
    Target &target;
    MotorController &motorController;
    PlausibilityValidator &plausibilityValidator;
    bool rawValuesChanged = false, plausibilityCheckFlagsChanged = false, useIttrFlagChanged = false;
    void loadRawValuesFromFlash();
    void loadPlausibilityCheckFlagsFromFlash();
    void loadUseIttrFlagFromFlash();
    void calibrate();
    void save();
    bool calibrating = false;
};

#endif
