#ifndef _CONFIGURATOR_H_
#define _CONFIGURATOR_H_

#include "sensors.hpp"
#include "motor_controller.hpp"
#include <ArduinoJson.h>
#include "flash.hpp"

#define JSON_SIZE 256

#define WAIT_INTERVAL 1000
#define CALIBRATE_INTERVAL 100

#define CALIBRATION_START_FLAG 's'
#define CALIBRATION_FINISH_FLAG 'f'
#define APPS_MIN_CALIBRATE_FLAG '1'
#define APPS_MAX_CALIBRATE_FLAG '2'
#define TPS_MIN_CALIBRATE_FLAG '3'
#define TPS_MAX_CALIBRATE_FLAG '4'

struct RawSensorValues
{
public:
    RawSensorValues();
    RawSensorValues(uint16_t apps1Min,
                    uint16_t apps1Max,
                    uint16_t apps2Min,
                    uint16_t apps2Max,
                    uint16_t ittrMin,
                    uint16_t ittrMax,
                    uint16_t tps1Min,
                    uint16_t tps1Max,
                    uint16_t tps2Min,
                    uint16_t tps2Max);
    Flash flash = Flash();
    uint16_t apps1Min, apps1Max, apps2Min, apps2Max, ittrMin, ittrMax, tps1Min, tps1Max, tps2Min, tps2Max;
    void initialize();
    void saveToFlash();
    void loadFromFlash();
    void loadFronJsonStr(const char *jsonStr);
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
    RawSensorValues rawValues = RawSensorValues();
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
    void calibrateFromConstants();
    void start();
    void finish();
};

void startWatingCalibration(void *calibrator);
#endif
