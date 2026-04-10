#include "configurator.hpp"

bool RawSensorValues::loadFromJsonStr(const char *jsonStr)
{
    StaticJsonDocument<RAW_SENSOR_VALUES_JSON_SIZE> json;
    DeserializationError error = deserializeJson(json, jsonStr);

    bool ok = !bool(error);
    ok &= json.containsKey("apps1Min");
    ok &= json.containsKey("apps1Max");
    ok &= json.containsKey("apps2Min");
    ok &= json.containsKey("apps2Max");
    ok &= json.containsKey("ittrMin");
    ok &= json.containsKey("ittrMax");
    ok &= json.containsKey("tps1Min");
    ok &= json.containsKey("tps1Max");
    ok &= json.containsKey("tps2Min");
    ok &= json.containsKey("tps2Max");
    ok &= json.containsKey("idling");
    if (!ok)
    {
        // メンバーが足りなかったときは何もせず False を返す
        return false;
    }
    apps1Min = json["apps1Min"];
    apps1Max = json["apps1Max"];
    apps2Min = json["apps2Min"];
    apps2Max = json["apps2Max"];
    ittrMin = json["ittrMin"];
    ittrMax = json["ittrMax"];
    tps1Min = json["tps1Min"];
    tps1Max = json["tps1Max"];
    tps2Min = json["tps2Min"];
    tps2Max = json["tps2Max"];
    idling = json["idling"];
    return true;
}

void RawSensorValues::loadFromConstants()
{
    apps1Min = APPS_1_RAW_MIN;
    apps1Max = APPS_1_RAW_MAX;
    apps2Min = APPS_2_RAW_MIN;
    apps2Max = APPS_2_RAW_MAX;
    ittrMin = ITTR_RAW_MIN;
    ittrMax = ITTR_RAW_MAX;
    tps1Min = TPS_1_RAW_MIN;
    tps1Max = TPS_1_RAW_MAX;
    tps2Min = TPS_2_RAW_MIN;
    tps2Max = TPS_2_RAW_MAX;
    idling = TARGET_IDLING;
}

String RawSensorValues::serialize()
{
    StaticJsonDocument<RAW_SENSOR_VALUES_JSON_SIZE> json;
    json["apps1Min"] = apps1Min;
    json["apps1Max"] = apps1Max;
    json["apps2Min"] = apps2Min;
    json["apps2Max"] = apps2Max;
    json["ittrMin"] = ittrMin;
    json["ittrMax"] = ittrMax;
    json["tps1Min"] = tps1Min;
    json["tps1Max"] = tps1Max;
    json["tps2Min"] = tps2Min;
    json["tps2Max"] = tps2Max;
    json["idling"] = idling;
    String out;
    serializeJson(json, out);
    return out;
}

bool PlausibilityCheckFlags::loadFromJsonStr(const char *jsonStr)
{
    StaticJsonDocument<PLAUSIBILITY_CHECK_FLAGS_JSON_SIZE> json;
    DeserializationError error = deserializeJson(json, jsonStr);

    bool ok = !bool(error);
    ok &= json.containsKey("apps");
    ok &= json.containsKey("tps");
    ok &= json.containsKey("apps1");
    ok &= json.containsKey("apps2");
    ok &= json.containsKey("tps1");
    ok &= json.containsKey("tps2");
    ok &= json.containsKey("target");
    ok &= json.containsKey("bps");
    ok &= json.containsKey("bpsTps");
    if (!ok)
    {
        // メンバーが足りなかったときは何もせず False を返す
        return false;
    }
    apps = json["apps"];
    tps = json["tps"];
    apps1 = json["apps1"];
    apps2 = json["apps2"];
    tps1 = json["tps1"];
    tps2 = json["tps2"];
    target = json["target"];
    bps = json["bps"];
    bpsTps = json["bpsTps"];
    return true;
}

void PlausibilityCheckFlags::loadFromConstants()
{
    apps = APPS_CHECK_FLAG;
    tps = TPS_CHECK_FLAG;
    apps1 = APPS1_CHECK_FLAG;
    apps2 = APPS2_CHECK_FLAG;
    tps1 = TPS1_CHECK_FLAG;
    tps2 = TPS2_CHECK_FLAG;
    target = TARGET_CHECK_FLAG;
    bps = BPS_CHECK_FLAG;
    bpsTps = BPSTPS_CHECK_FLAG;
}

String PlausibilityCheckFlags::serialize()
{
    StaticJsonDocument<PLAUSIBILITY_CHECK_FLAGS_JSON_SIZE> json;
    json["apps"] = apps;
    json["tps"] = tps;
    json["apps1"] = apps1;
    json["apps2"] = apps2;
    json["tps1"] = tps1;
    json["tps2"] = tps2;
    json["target"] = target;
    json["bps"] = bps;
    json["bpsTps"] = bpsTps;
    String out;
    serializeJson(json, out);
    return out;
}

bool UseIttrFlag::loadFromJsonStr(const char *jsonStr)
{
    StaticJsonDocument<USE_ITTR_FLAG_JSON_SIZE> json;
    DeserializationError error = deserializeJson(json, jsonStr);

    bool ok = !bool(error);
    ok &= json.containsKey("useIttr");
    if (!ok)
    {
        // メンバーが足りなかったときは何もせず False を返す
        return false;
    }
    useIttr = json["useIttr"];
    return true;
}

void UseIttrFlag::loadFromConstants()
{
#ifdef IST_CONTROLLER
    useIttr = true;
#else
    useIttr = false;
#endif
}

String UseIttrFlag::serialize()
{
    StaticJsonDocument<USE_ITTR_FLAG_JSON_SIZE> json;
    json["useIttr"] = useIttr;
    String out;
    serializeJson(json, out);
    return out;
}

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
    apps1.setRawMin(rawValues.apps1Min);
    apps1.setRawMax(rawValues.apps1Max);
    apps2.setRawMin(rawValues.apps2Min);
    apps2.setRawMax(rawValues.apps2Max);
    ittr.setRawMin(rawValues.ittrMin);
    ittr.setRawMax(rawValues.ittrMax);
    target.setIdlingValue(rawValues.idling);
    tps1.setRawMin(rawValues.tps1Min);
    tps1.setRawMax(rawValues.tps1Max);
    tps2.setRawMin(rawValues.tps2Min);
    tps2.setRawMax(rawValues.tps2Max);
    plausibilityValidator.setCheckFlags(
        plausibilityCheckFlags.apps, plausibilityCheckFlags.tps, plausibilityCheckFlags.apps1,
        plausibilityCheckFlags.apps2, plausibilityCheckFlags.tps1, plausibilityCheckFlags.tps2,
        plausibilityCheckFlags.target, plausibilityCheckFlags.bps, plausibilityCheckFlags.bpsTps);
    target.setIttr(useIttrFlag.useIttr);
}

void Configurator::loadRawValuesFromFlash()
{
    String jsonStr = flash.read(SENSOR_VALUES_FILE_NAME);
    if (!rawValues.loadFromJsonStr(jsonStr.c_str()))
    {
        // False のときは Constants から読み込む。
        rawValues.loadFromConstants();
    }
}

void Configurator::loadPlausibilityCheckFlagsFromFlash()
{
    String jsonStr = flash.read(PLAUSIBILITY_CHECK_FLAGS_FILE_NAME);
    if (!plausibilityCheckFlags.loadFromJsonStr(jsonStr.c_str()))
    {
        // False のときは Constants から読み込む。
        plausibilityCheckFlags.loadFromConstants();
    }
}

void Configurator::loadUseIttrFlagFromFlash()
{
    String jsonStr = flash.read(USE_ITTR_FLAG_FILE_NAME);
    if (!useIttrFlag.loadFromJsonStr(jsonStr.c_str()))
    {
        // False のときは Constants から読み込む。
        useIttrFlag.loadFromConstants();
    }
}

void Configurator::calibrateFromFlash()
{
    loadRawValuesFromFlash();
    loadPlausibilityCheckFlagsFromFlash();
    loadUseIttrFlagFromFlash();
    calibrate();
}

void Configurator::setPlausibilityFlags(const PlausibilityCheckFlags &flags)
{
    plausibilityCheckFlags = flags;
    plausibilityValidator.setCheckFlags(flags.apps, flags.tps, flags.apps1, flags.apps2, flags.tps1, flags.tps2,
                                        flags.target, flags.bps, flags.bpsTps);
    plausibilityCheckFlagsChanged = true;
}

void Configurator::setIttrFlag(bool val)
{
    useIttrFlag.useIttr = val;
    target.setIttr(val);
    useIttrFlagChanged = true;
}

bool Configurator::importConfig(const char *jsonStr)
{
    if (!rawValues.loadFromJsonStr(jsonStr))
    {
        return false;
    }
    calibrate();
    rawValuesChanged = true;
    save();
    return true;
}

void Configurator::setAppsMin()
{
    rawValues.apps1Min = apps1.setCurrentValRawMin();
    rawValues.apps2Min = apps2.setCurrentValRawMin();
    rawValues.ittrMin = ittr.setCurrentValRawMin();
    rawValuesChanged = true;
}

void Configurator::setAppsMax()
{
    rawValues.apps1Max = apps1.setCurrentValRawMax();
    rawValues.apps2Max = apps2.setCurrentValRawMax();
    rawValues.ittrMax = ittr.setCurrentValRawMax();
    rawValuesChanged = true;
}

void Configurator::setTpsMin()
{
    rawValues.tps1Min = tps1.setCurrentValRawMin();
    rawValues.tps2Min = tps2.setCurrentValRawMin();
    rawValuesChanged = true;
}

void Configurator::setTpsMax()
{
    rawValues.tps1Max = tps1.setCurrentValRawMax();
    rawValues.tps2Max = tps2.setCurrentValRawMax();
    rawValuesChanged = true;
}

void Configurator::setIdling()
{
    rawValues.idling = tps1.convertedValue();
    target.setIdlingValue(rawValues.idling);
    rawValuesChanged = true;
}

void Configurator::save()
{
    if (rawValuesChanged)
    {
        flash.write(SENSOR_VALUES_FILE_NAME, rawValues.serialize());
    }
    if (plausibilityCheckFlagsChanged)
    {
        flash.write(PLAUSIBILITY_CHECK_FLAGS_FILE_NAME, plausibilityCheckFlags.serialize());
    }
    if (useIttrFlagChanged)
    {
        flash.write(USE_ITTR_FLAG_FILE_NAME, useIttrFlag.serialize());
    }
    rawValuesChanged = false;
    plausibilityCheckFlagsChanged = false;
    useIttrFlagChanged = false;
}

void Configurator::revert()
{
    calibrateFromFlash();
    rawValuesChanged = false;
    plausibilityCheckFlagsChanged = false;
    useIttrFlagChanged = false;
}

void Configurator::getConfigJson(JsonObject &out)
{
    JsonObject sv = out.createNestedObject("sensorValues");
    sv["apps1Min"] = rawValues.apps1Min;
    sv["apps1Max"] = rawValues.apps1Max;
    sv["apps2Min"] = rawValues.apps2Min;
    sv["apps2Max"] = rawValues.apps2Max;
    sv["ittrMin"] = rawValues.ittrMin;
    sv["ittrMax"] = rawValues.ittrMax;
    sv["tps1Min"] = rawValues.tps1Min;
    sv["tps1Max"] = rawValues.tps1Max;
    sv["tps2Min"] = rawValues.tps2Min;
    sv["tps2Max"] = rawValues.tps2Max;
    sv["idling"] = rawValues.idling;

    JsonObject pf = out.createNestedObject("plausibilityFlags");
    pf["apps"] = plausibilityCheckFlags.apps;
    pf["tps"] = plausibilityCheckFlags.tps;
    pf["apps1"] = plausibilityCheckFlags.apps1;
    pf["apps2"] = plausibilityCheckFlags.apps2;
    pf["tps1"] = plausibilityCheckFlags.tps1;
    pf["tps2"] = plausibilityCheckFlags.tps2;
    pf["target"] = plausibilityCheckFlags.target;
    pf["bps"] = plausibilityCheckFlags.bps;
    pf["bpsTps"] = plausibilityCheckFlags.bpsTps;

    out["useIttr"] = useIttrFlag.useIttr;
}