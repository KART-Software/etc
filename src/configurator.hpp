#ifndef _CONFIGURATOR_H_
#define _CONFIGURATOR_H_

#include "sensors.hpp"
#include "motor_controller.hpp"
#include <ArduinoJson.h>
#include "flash.hpp"

#define SENSOR_VALUES_FILE_NAME "/sensor_values.txt"
#define RAW_SENSOR_VALUES_JSON_SIZE 256

#define WAIT_INTERVAL 1000
#define CALIBRATE_INTERVAL 100

#define CALIBRATION_START_KEY 's'
#define CALIBRATION_FINISH_KEY 'f'
#define MOTOR_OFF_KEY 'm'
#define APPS_MIN_CALIBRATE_KEY '1'
#define APPS_MAX_CALIBRATE_KEY '2'
#define TPS_MIN_CALIBRATE_KEY '3'
#define TPS_MAX_CALIBRATE_KEY '4'

struct Config
{
public:
    virtual bool loadFromJsonStr(const char *jsonStr);
    virtual void loadFronConstants();
    virtual const char *toJsonStr();
};

struct RawSensorValues : Config
{
public:
    uint16_t apps1Min, apps1Max, apps2Min, apps2Max, ittrMin, ittrMax, tps1Min, tps1Max, tps2Min, tps2Max;
    bool loadFromJsonStr(const char *jsonStr);
    void loadFronConstants();
    const char *toJsonStr();
};

class Configurator
{
public:
    Configurator(Apps &apps1, Apps &apps2, Tps &tps1, Tps &tps2, Ittr &ittr, MotorController &motorController);
    void initialize();
    void calibrateFromFlash();
    void startWaiting();

private:
    Flash flash;
    RawSensorValues rawValues;
    Apps &apps1, &apps2;
    Tps &tps1, &tps2;
    Ittr &ittr;
    MotorController &motorController;
    void setAppsMin();
    void setAppsMax();
    void setTpsMin();
    void setTpsMax();
    void calibrate();
    void calibrate(char c);
    void start();
    void finish();
};

void startWatingCalibration(void *configurator);
#endif
