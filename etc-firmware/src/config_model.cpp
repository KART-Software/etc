#include "config_model.hpp"

void ConfigModel::loadFromConstants()
{
    sensorValues.apps1Min = APPS_1_RAW_MIN;
    sensorValues.apps1Max = APPS_1_RAW_MAX;
    sensorValues.apps2Min = APPS_2_RAW_MIN;
    sensorValues.apps2Max = APPS_2_RAW_MAX;
    sensorValues.ittrMin = ITTR_RAW_MIN;
    sensorValues.ittrMax = ITTR_RAW_MAX;
    sensorValues.tps1Min = TPS_1_RAW_MIN;
    sensorValues.tps1Max = TPS_1_RAW_MAX;
    sensorValues.tps2Min = TPS_2_RAW_MIN;
    sensorValues.tps2Max = TPS_2_RAW_MAX;
    sensorValues.idling = TARGET_IDLING;
    sensorValues.normalMax = TARGET_NORMAL_MAX;
    sensorValues.restrictedMax = TARGET_RESTRICTED_MAX;

    plausibilityFlags.apps = APPS_CHECK_FLAG;
    plausibilityFlags.tps = TPS_CHECK_FLAG;
    plausibilityFlags.apps1 = APPS1_CHECK_FLAG;
    plausibilityFlags.apps2 = APPS2_CHECK_FLAG;
    plausibilityFlags.tps1 = TPS1_CHECK_FLAG;
    plausibilityFlags.tps2 = TPS2_CHECK_FLAG;
    plausibilityFlags.target = TARGET_CHECK_FLAG;
    plausibilityFlags.bps = BPS_CHECK_FLAG;
    plausibilityFlags.bpsTps = BPSTPS_CHECK_FLAG;

#ifdef IST_CONTROLLER
    useIttr = true;
#else
    useIttr = false;
#endif

    pid.kP = KP;
    pid.kI = KI;
    pid.kD = KD;

    targetCurve.a4 = TARGET_CURVE_A4;
    targetCurve.a3 = TARGET_CURVE_A3;
    targetCurve.a2 = TARGET_CURVE_A2;
    targetCurve.a1 = TARGET_CURVE_A1;
}

bool ConfigModel::loadFromJson(const String &jsonStr)
{
    StaticJsonDocument<CONFIG_JSON_SIZE> doc;
    DeserializationError error = deserializeJson(doc, jsonStr);
    if (error)
    {
        return false;
    }

    // sensorValues
    if (!doc.containsKey("sensorValues"))
    {
        return false;
    }
    JsonObject sv = doc["sensorValues"];
    bool svOk = sv.containsKey("apps1Min") && sv.containsKey("apps1Max") &&
                sv.containsKey("apps2Min") && sv.containsKey("apps2Max") &&
                sv.containsKey("ittrMin") && sv.containsKey("ittrMax") &&
                sv.containsKey("tps1Min") && sv.containsKey("tps1Max") &&
                sv.containsKey("tps2Min") && sv.containsKey("tps2Max") &&
                sv.containsKey("idling");
    if (!svOk)
    {
        return false;
    }
    sensorValues.apps1Min = sv["apps1Min"];
    sensorValues.apps1Max = sv["apps1Max"];
    sensorValues.apps2Min = sv["apps2Min"];
    sensorValues.apps2Max = sv["apps2Max"];
    sensorValues.ittrMin = sv["ittrMin"];
    sensorValues.ittrMax = sv["ittrMax"];
    sensorValues.tps1Min = sv["tps1Min"];
    sensorValues.tps1Max = sv["tps1Max"];
    sensorValues.tps2Min = sv["tps2Min"];
    sensorValues.tps2Max = sv["tps2Max"];
    sensorValues.idling = sv["idling"];
    sensorValues.normalMax = sv["normalMax"] | (double)TARGET_NORMAL_MAX;
    sensorValues.restrictedMax = sv["restrictedMax"] | (double)TARGET_RESTRICTED_MAX;

    // plausibilityFlags
    if (!doc.containsKey("plausibilityFlags"))
    {
        return false;
    }
    JsonObject pf = doc["plausibilityFlags"];
    bool pfOk = pf.containsKey("apps") && pf.containsKey("tps") &&
                pf.containsKey("apps1") && pf.containsKey("apps2") &&
                pf.containsKey("tps1") && pf.containsKey("tps2") &&
                pf.containsKey("target") && pf.containsKey("bps") &&
                pf.containsKey("bpsTps");
    if (!pfOk)
    {
        return false;
    }
    plausibilityFlags.apps = pf["apps"];
    plausibilityFlags.tps = pf["tps"];
    plausibilityFlags.apps1 = pf["apps1"];
    plausibilityFlags.apps2 = pf["apps2"];
    plausibilityFlags.tps1 = pf["tps1"];
    plausibilityFlags.tps2 = pf["tps2"];
    plausibilityFlags.target = pf["target"];
    plausibilityFlags.bps = pf["bps"];
    plausibilityFlags.bpsTps = pf["bpsTps"];

    // useIttr
    if (!doc.containsKey("useIttr"))
    {
        return false;
    }
    useIttr = doc["useIttr"];

    // pid
    if (doc.containsKey("pid"))
    {
        JsonObject p = doc["pid"];
        pid.kP = p["kP"] | (double)KP;
        pid.kI = p["kI"] | (double)KI;
        pid.kD = p["kD"] | (double)KD;
    }
    else
    {
        pid.kP = KP;
        pid.kI = KI;
        pid.kD = KD;
    }

    // targetCurve (optional)
    if (doc.containsKey("targetCurve"))
    {
        JsonObject tc = doc["targetCurve"];
        targetCurve.a4 = tc["a4"] | (double)TARGET_CURVE_A4;
        targetCurve.a3 = tc["a3"] | (double)TARGET_CURVE_A3;
        targetCurve.a2 = tc["a2"] | (double)TARGET_CURVE_A2;
        targetCurve.a1 = tc["a1"] | (double)TARGET_CURVE_A1;
    }
    else
    {
        targetCurve.a4 = TARGET_CURVE_A4;
        targetCurve.a3 = TARGET_CURVE_A3;
        targetCurve.a2 = TARGET_CURVE_A2;
        targetCurve.a1 = TARGET_CURVE_A1;
    }

    return true;
}

void ConfigModel::toJson(JsonObject &out) const
{
    JsonObject sv = out.createNestedObject("sensorValues");
    sv["apps1Min"] = sensorValues.apps1Min;
    sv["apps1Max"] = sensorValues.apps1Max;
    sv["apps2Min"] = sensorValues.apps2Min;
    sv["apps2Max"] = sensorValues.apps2Max;
    sv["ittrMin"] = sensorValues.ittrMin;
    sv["ittrMax"] = sensorValues.ittrMax;
    sv["tps1Min"] = sensorValues.tps1Min;
    sv["tps1Max"] = sensorValues.tps1Max;
    sv["tps2Min"] = sensorValues.tps2Min;
    sv["tps2Max"] = sensorValues.tps2Max;
    sv["idling"] = sensorValues.idling;
    sv["normalMax"] = sensorValues.normalMax;
    sv["restrictedMax"] = sensorValues.restrictedMax;

    JsonObject pf = out.createNestedObject("plausibilityFlags");
    pf["apps"] = plausibilityFlags.apps;
    pf["tps"] = plausibilityFlags.tps;
    pf["apps1"] = plausibilityFlags.apps1;
    pf["apps2"] = plausibilityFlags.apps2;
    pf["tps1"] = plausibilityFlags.tps1;
    pf["tps2"] = plausibilityFlags.tps2;
    pf["target"] = plausibilityFlags.target;
    pf["bps"] = plausibilityFlags.bps;
    pf["bpsTps"] = plausibilityFlags.bpsTps;

    out["useIttr"] = useIttr;

    JsonObject p = out.createNestedObject("pid");
    p["kP"] = pid.kP;
    p["kI"] = pid.kI;
    p["kD"] = pid.kD;

    JsonObject tc = out.createNestedObject("targetCurve");
    tc["a4"] = targetCurve.a4;
    tc["a3"] = targetCurve.a3;
    tc["a2"] = targetCurve.a2;
    tc["a1"] = targetCurve.a1;
}
