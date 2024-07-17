#ifndef _CONFIGURATOR_H_
#define _CONFIGURATOR_H_

#include "sensors.hpp"
#include "motor_controller.hpp"
#include <ArduinoJson.h>
#include "flash.hpp"
#include "plausibility_validator.hpp"

#define SENSOR_VALUES_FILE_NAME "/sensor_values.txt"
#define PLAUSIBILITY_CHECK_FLAGS_FILE_NAME "/plausibility_check_flags.txt"
#define USE_ITTR_FLAG_FILE_NAME "/use_ittr_flag.txt"
#define RAW_SENSOR_VALUES_JSON_SIZE 300
#define PLAUSIBILITY_CHECK_FLAGS_JSON_SIZE 300
#define USE_ITTR_FLAG_JSON_SIZE 50

#define WAIT_INTERVAL 1000
#define CALIBRATE_INTERVAL 100

#define CALIBRATION_START_KEY 's'
#define CALIBRATION_FINISH_KEY 'f'
#define MOTOR_OFF_KEY 'm'
#define APPS_MIN_CALIBRATE_KEY '1'
#define APPS_MAX_CALIBRATE_KEY '2'
#define TPS_MIN_CALIBRATE_KEY '3'
#define TPS_MAX_CALIBRATE_KEY '4'
#define IDLING_CALIBRATE_KEY '5'

#define APPS_CHECK_FLAG_SET_KEY 'q'
#define TPS_CHECK_FLAG_SET_KEY 'w'
#define APPS1_CHECK_FLAG_SET_KEY 'e'
#define APPS2_CHECK_FLAG_SET_KEY 'r'
#define TPS1_CHECK_FLAG_SET_KEY 't'
#define TPS2_CHECK_FLAG_SET_KEY 'y'
#define TARGET_CHECK_FLAG_SET_KEY 'u'
#define BPS_CHECK_FLAG_SET_KEY 'i'
#define BPSTPS_CHECK_FLAG_SET_KEY 'o'

#define IST_CONTROLLER_SET_KEY 'x'

#define REBOOT_KEY 'z'

struct Config
{
public:
    virtual bool loadFromJsonStr(const char *jsonStr);
    virtual void loadFromConstants();
    virtual const char *toJsonStr();
};

struct RawSensorValues : Config
{
public:
    uint16_t apps1Min, apps1Max, apps2Min, apps2Max, ittrMin, ittrMax, tps1Min, tps1Max, tps2Min, tps2Max;
    double idling;
    bool loadFromJsonStr(const char *jsonStr);
    void loadFromConstants();
    const char *toJsonStr();
};

struct PlausibilityCheckFlags : Config
{
public:
    bool apps, tps, apps1, apps2, tps1, tps2, target, bps, bpsTps;
    bool loadFromJsonStr(const char *jsonStr);
    void loadFromConstants();
    const char *toJsonStr();
};

struct UseIttrFlag : Config
{
public:
    bool useIttr;
    bool loadFromJsonStr(const char *jsonStr);
    void loadFromConstants();
    const char *toJsonStr();
};

class Configurator
{
public:
    Configurator(Apps &apps1, Apps &apps2, Tps &tps1, Tps &tps2, Ittr &ittr, Target &target, MotorController &motorController, PlausibilityValidator &plausibilityValidator);
    void initialize();
    void loadRawValuesFromFlash();
    void loadPlausibilityCheckFlagsFromFlash();
    void loadUseIttrFlagFromFlash();
    void calibrateFromFlash();
    void startWaiting();

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
    bool rawValuesChanged, plausibilityCheckFlagsChanged, useIttrFlagChanged;
    void setAppsMin();
    void setAppsMax();
    void setTpsMin();
    void setTpsMax();
    void setIdling();
    void calibrate();
    void calibrate(char c);
    void start();
    void finish();
};

void startWatingCalibration(void *configurator);
#endif
