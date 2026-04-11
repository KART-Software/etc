#ifndef _CONFIGURATOR_H_
#define _CONFIGURATOR_H_

#include "config_model.hpp"
#include "sensors.hpp"
#include "motor_controller.hpp"
#include "flash.hpp"
#include "plausibility_validator.hpp"
#include "serial_protocol.hpp"

class Configurator
{
public:
    Configurator(Apps &apps1, Apps &apps2, Tps &tps1, Tps &tps2, Ittr &ittr, Target &target,
                 MotorController &motorController, PlausibilityValidator &plausibilityValidator);
    void initialize();
    void calibrateFromFlash();
    void getConfigJson(JsonObject &out);

    void setAppsMin();
    void setAppsMax();
    void setTpsMin();
    void setTpsMax();
    void setIdling();
    void setTargetBound(double idling, double normalMax, double restrictedMax);
    void setPlausibilityFlags(const PlausibilityCheckFlags &flags);
    void setIttrFlag(bool val);
    void setPid(double kP, double kI, double kD);
    void setTargetCurve(const TargetCurve &curve);
    bool importConfig(const char *jsonStr);
    void save();
    void revert();
    Flash flash;
    ConfigModel config;
    Apps &apps1, &apps2;
    Tps &tps1, &tps2;
    Ittr &ittr;
    Target &target;
    MotorController &motorController;
    PlausibilityValidator &plausibilityValidator;
    bool configChanged = false;
    void loadConfigFromFlash();
    void calibrate();
};

#endif
