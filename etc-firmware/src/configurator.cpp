#include "configurator.hpp"

Configurator::Configurator(Apps &apps1, Apps &apps2, Tps &tps1, Tps &tps2, Ittr &ittr, Target &target,
                           MotorController &motorController, PlausibilityValidator &plausibilityValidator)
    : apps1(apps1), apps2(apps2), tps1(tps1), tps2(tps2), ittr(ittr), target(target), motorController(motorController),
      plausibilityValidator(plausibilityValidator)
{
}

void Configurator::initialize()
{
    flash.initialize();
    // TODO initialize 失敗時の処理
}

void Configurator::calibrate()
{
    apps1.setRawMin(config.sensorValues.apps1Min);
    apps1.setRawMax(config.sensorValues.apps1Max);
    apps2.setRawMin(config.sensorValues.apps2Min);
    apps2.setRawMax(config.sensorValues.apps2Max);
    ittr.setRawMin(config.sensorValues.ittrMin);
    ittr.setRawMax(config.sensorValues.ittrMax);
    target.setIdlingValue(config.sensorValues.idling);
    tps1.setRawMin(config.sensorValues.tps1Min);
    tps1.setRawMax(config.sensorValues.tps1Max);
    tps2.setRawMin(config.sensorValues.tps2Min);
    tps2.setRawMax(config.sensorValues.tps2Max);
    plausibilityValidator.setCheckFlags(
        config.plausibilityFlags.apps, config.plausibilityFlags.tps, config.plausibilityFlags.apps1,
        config.plausibilityFlags.apps2, config.plausibilityFlags.tps1, config.plausibilityFlags.tps2,
        config.plausibilityFlags.target, config.plausibilityFlags.bps, config.plausibilityFlags.bpsTps);
    target.setIttr(config.useIttr);
    motorController.setPidGains(config.pid.kP, config.pid.kI, config.pid.kD);
    target.setTargetCurve(config.targetCurve);
}

void Configurator::loadConfigFromFlash()
{
    String jsonStr = flash.read(CONFIG_FILE_NAME);
    if (!config.loadFromJson(jsonStr))
    {
        config.loadFromConstants();
    }
}

void Configurator::calibrateFromFlash()
{
    loadConfigFromFlash();
    calibrate();
}

void Configurator::setPlausibilityFlags(const PlausibilityCheckFlags &flags)
{
    config.plausibilityFlags = flags;
    plausibilityValidator.setCheckFlags(flags.apps, flags.tps, flags.apps1, flags.apps2, flags.tps1, flags.tps2,
                                        flags.target, flags.bps, flags.bpsTps);
    configChanged = true;
}

void Configurator::setIttrFlag(bool val)
{
    config.useIttr = val;
    target.setIttr(val);
    configChanged = true;
}

void Configurator::setPid(double kP, double kI, double kD)
{
    config.pid.kP = kP;
    config.pid.kI = kI;
    config.pid.kD = kD;
    motorController.setPidGains(kP, kI, kD);
    configChanged = true;
}

void Configurator::setTargetCurve(const TargetCurve &curve)
{
    config.targetCurve = curve;
    target.setTargetCurve(curve);
    configChanged = true;
}

bool Configurator::importConfig(const char *jsonStr)
{
    ConfigModel tmp = config;
    if (!tmp.loadFromJson(jsonStr))
    {
        return false;
    }
    config = tmp;
    calibrate();
    configChanged = true;
    save();
    return true;
}

void Configurator::setAppsMin()
{
    config.sensorValues.apps1Min = apps1.setCurrentValRawMin();
    config.sensorValues.apps2Min = apps2.setCurrentValRawMin();
    config.sensorValues.ittrMin = ittr.setCurrentValRawMin();
    configChanged = true;
}

void Configurator::setAppsMax()
{
    config.sensorValues.apps1Max = apps1.setCurrentValRawMax();
    config.sensorValues.apps2Max = apps2.setCurrentValRawMax();
    config.sensorValues.ittrMax = ittr.setCurrentValRawMax();
    configChanged = true;
}

void Configurator::setTpsMin()
{
    config.sensorValues.tps1Min = tps1.setCurrentValRawMin();
    config.sensorValues.tps2Min = tps2.setCurrentValRawMin();
    configChanged = true;
}

void Configurator::setTpsMax()
{
    config.sensorValues.tps1Max = tps1.setCurrentValRawMax();
    config.sensorValues.tps2Max = tps2.setCurrentValRawMax();
    configChanged = true;
}

void Configurator::setIdling()
{
    config.sensorValues.idling = tps1.convertedValue();
    target.setIdlingValue(config.sensorValues.idling);
    configChanged = true;
}

void Configurator::save()
{
    if (configChanged)
    {
        StaticJsonDocument<CONFIG_JSON_SIZE> doc;
        JsonObject root = doc.to<JsonObject>();
        config.toJson(root);
        String out;
        serializeJson(doc, out);
        flash.write(CONFIG_FILE_NAME, out);
    }
    configChanged = false;
}

void Configurator::revert()
{
    calibrateFromFlash();
    configChanged = false;
}

void Configurator::getConfigJson(JsonObject &out)
{
    config.toJson(out);
    out["configChanged"] = configChanged;
}