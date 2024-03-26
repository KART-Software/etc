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
    idling = APPS_IDLING;
}

const char *RawSensorValues::toJsonStr()
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
    char *jsonStr = new char[RAW_SENSOR_VALUES_JSON_SIZE];
    serializeJson(json, jsonStr, RAW_SENSOR_VALUES_JSON_SIZE);
    return jsonStr;
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

const char *PlausibilityCheckFlags::toJsonStr()
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
    char *jsonStr = new char[PLAUSIBILITY_CHECK_FLAGS_JSON_SIZE];
    serializeJson(json, jsonStr, PLAUSIBILITY_CHECK_FLAGS_JSON_SIZE);
    return jsonStr;
}

Configurator::Configurator(Apps &apps1, Apps &apps2, Tps &tps1, Tps &tps2, Ittr &ittr, MotorController &motorController, PlausibilityValidator &plausibilityValidator)
    : apps1(apps1), apps2(apps2), tps1(tps1), tps2(tps2), ittr(ittr), motorController(motorController), plausibilityValidator(plausibilityValidator)
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
    apps1.setIdlingValue(rawValues.idling);
    apps1.setIdlingValue(rawValues.idling);
    apps2.setIdlingValue(rawValues.idling);
    apps2.setIdlingValue(rawValues.idling);
    ittr.setIdlingValue(rawValues.idling);
    ittr.setIdlingValue(rawValues.idling);
    tps1.setRawMin(rawValues.tps1Min);
    tps1.setRawMax(rawValues.tps1Max);
    tps2.setRawMin(rawValues.tps2Min);
    tps2.setRawMax(rawValues.tps2Max);
    plausibilityValidator.setCheckFlags(
        plausibilityCheckFlags.apps,
        plausibilityCheckFlags.tps,
        plausibilityCheckFlags.apps1,
        plausibilityCheckFlags.apps2,
        plausibilityCheckFlags.tps1,
        plausibilityCheckFlags.tps2,
        plausibilityCheckFlags.target,
        plausibilityCheckFlags.bps,
        plausibilityCheckFlags.bpsTps);
}

void Configurator::loadRawValuesFromFlash()
{
    const char *jsonStr = flash.read(SENSOR_VALUES_FILE_NAME);
    if (!rawValues.loadFromJsonStr(jsonStr))
    {
        // False のときは Constants から読み込む。
        rawValues.loadFromConstants();
    }
}

void Configurator::loadPlausibilityCheckFlagsFromFlash()
{
    const char *jsonStr = flash.read(PLAUSIBILITY_CHECK_FLAGS_FILE_NAME);
    if (!plausibilityCheckFlags.loadFromJsonStr(jsonStr))
    {
        // False のときは Constants から読み込む。
        plausibilityCheckFlags.loadFromConstants();
    }
}

void Configurator::calibrateFromFlash()
{
    loadRawValuesFromFlash();
    loadPlausibilityCheckFlagsFromFlash();
    calibrate();
}

void Configurator::calibrate(char c)
{
    switch (c)
    {
    case MOTOR_OFF_KEY:
        motorController.setMotorOff();
        Serial.println("\033[K---- Motor Off ----");
        break;
    case APPS_MIN_CALIBRATE_KEY:
        setAppsMin();
        Serial.println("\033[K---- APPS Min Set ----");
        rawValuesChanged = true;
        break;
    case APPS_MAX_CALIBRATE_KEY:
        setAppsMax();
        Serial.println("\033[K---- APPS Max Set ----");
        rawValuesChanged = true;
        break;
    case TPS_MIN_CALIBRATE_KEY:
        setTpsMin();
        Serial.println("\033[K---- TPS Min Set ----");
        rawValuesChanged = true;
        break;
    case TPS_MAX_CALIBRATE_KEY:
        setTpsMax();
        Serial.println("\033[K---- TPS Max Set ----");
        rawValuesChanged = true;
        break;
    case IDLING_CALIBRATE_KEY:
        setIdling();
        Serial.println("\033[K---- Idling Set ----");
        rawValuesChanged = true;
        break;
    case APPS_CHECK_FLAG_SET_KEY:
        plausibilityCheckFlags.apps = !plausibilityCheckFlags.apps;
        plausibilityValidator.appsCheckFlag = plausibilityCheckFlags.apps;
        Serial.printf("\033[K---- APPS Check: %d ----\n", plausibilityCheckFlags.apps);
        plausibilityCheckFlagsChanged = true;
        break;
    case TPS_CHECK_FLAG_SET_KEY:
        plausibilityCheckFlags.tps = !plausibilityCheckFlags.tps;
        plausibilityValidator.tpsCheckFlag = plausibilityCheckFlags.tps;
        Serial.printf("\033[K---- TPPS Check: %d ----\n", plausibilityCheckFlags.tps);
        plausibilityCheckFlagsChanged = true;
        break;
    case APPS1_CHECK_FLAG_SET_KEY:
        plausibilityCheckFlags.apps1 = !plausibilityCheckFlags.apps1;
        plausibilityValidator.apps1CheckFlag = plausibilityCheckFlags.apps1;
        Serial.printf("\033[K---- APPS1 Check: %d ----\n", plausibilityCheckFlags.apps1);
        plausibilityCheckFlagsChanged = true;
        break;
    case APPS2_CHECK_FLAG_SET_KEY:
        plausibilityCheckFlags.apps2 = !plausibilityCheckFlags.apps2;
        plausibilityValidator.apps2CheckFlag = plausibilityCheckFlags.apps2;
        Serial.printf("\033[K---- APPS2 Check: %d ----\n", plausibilityCheckFlags.apps2);
        plausibilityCheckFlagsChanged = true;
        break;
    case TPS1_CHECK_FLAG_SET_KEY:
        plausibilityCheckFlags.tps1 = !plausibilityCheckFlags.tps1;
        plausibilityValidator.tps1CheckFlag = plausibilityCheckFlags.tps1;
        Serial.printf("\033[K---- TPS1 Check: %d ----\n", plausibilityCheckFlags.tps1);
        plausibilityCheckFlagsChanged = true;
        break;
    case TPS2_CHECK_FLAG_SET_KEY:
        plausibilityCheckFlags.tps2 = !plausibilityCheckFlags.tps2;
        plausibilityValidator.tps2CheckFlag = plausibilityCheckFlags.tps2;
        Serial.printf("\033[K---- TPS2 Check: %d ----\n", plausibilityCheckFlags.tps2);
        plausibilityCheckFlagsChanged = true;
        break;
    case TARGET_CHECK_FLAG_SET_KEY:
        plausibilityCheckFlags.target = !plausibilityCheckFlags.target;
        plausibilityValidator.targetCheckFlag = plausibilityCheckFlags.target;
        Serial.printf("\033[K---- TARGET Check: %d ----\n", plausibilityCheckFlags.target);
        plausibilityCheckFlagsChanged = true;
        break;
    case BPS_CHECK_FLAG_SET_KEY:
        plausibilityCheckFlags.bps = !plausibilityCheckFlags.bps;
        plausibilityValidator.bpsCheckFlag = plausibilityCheckFlags.bps;
        Serial.printf("\033[K---- BPS Check: %d ----\n", plausibilityCheckFlags.bps);
        plausibilityCheckFlagsChanged = true;
        break;
    case BPSTPS_CHECK_FLAG_SET_KEY:
        plausibilityCheckFlags.bpsTps = !plausibilityCheckFlags.bpsTps;
        plausibilityValidator.bpsTpsCheckFlag = plausibilityCheckFlags.bpsTps;
        Serial.printf("\033[K---- BPSTPS Check: %d ----\n", plausibilityCheckFlags.bpsTps);
        plausibilityCheckFlagsChanged = true;
        break;
    case CALIBRATION_FINISH_KEY:
        Serial.println("\033[K---- Calibration Finish ----");
        finish();
        break;
    case REBOOT_KEY:
        Serial.println("\033[K---- Reboot ----");
        ESP.restart();
    default:
        break;
    }
}

void Configurator::setAppsMin()
{
    rawValues.apps1Min = apps1.setCurrentValRawMin();
    rawValues.apps2Min = apps2.setCurrentValRawMin();
    rawValues.ittrMin = ittr.setCurrentValRawMin();
}

void Configurator::setAppsMax()
{
    rawValues.apps1Max = apps1.setCurrentValRawMax();
    rawValues.apps2Max = apps2.setCurrentValRawMax();
    rawValues.ittrMax = ittr.setCurrentValRawMax();
}

void Configurator::setTpsMin()
{
    rawValues.tps1Min = tps1.setCurrentValRawMin();
    rawValues.tps2Min = tps2.setCurrentValRawMin();
}

void Configurator::setTpsMax()
{
    rawValues.tps1Max = tps1.setCurrentValRawMax();
    rawValues.tps2Max = tps2.setCurrentValRawMax();
}

void Configurator::setIdling()
{
    rawValues.idling = tps1.convertedValue();
    apps1.setIdlingValue(rawValues.idling);
    apps1.setIdlingValue(rawValues.idling);
    apps2.setIdlingValue(rawValues.idling);
    apps2.setIdlingValue(rawValues.idling);
    ittr.setIdlingValue(rawValues.idling);
    ittr.setIdlingValue(rawValues.idling);
}

void Configurator::start()
{
    rawValuesChanged = false;
    plausibilityCheckFlagsChanged = false;
    while (true)
    {
        if (Serial.available())
        {
            char c = Serial.read();
            while (Serial.available())
            {
                Serial.read();
            }
            calibrate(c);
        }
        delay(CALIBRATE_INTERVAL);
    }
}

void Configurator::startWaiting()
{
    while (true)
    {
        if (Serial.available())
        {
            if (Serial.read() == CALIBRATION_START_KEY)
            {
                while (Serial.available())
                {
                    Serial.read();
                }
                Serial.println("\033[K---- Calibration Start ----");
                start();
            }
            else
            {
                while (Serial.available())
                {
                    Serial.read();
                }
            }
        }
        delay(WAIT_INTERVAL);
    }
}

void Configurator::finish()
{
    if (rawValuesChanged)
    {
        flash.write(SENSOR_VALUES_FILE_NAME, rawValues.toJsonStr());
    }
    if (plausibilityCheckFlagsChanged)
    {
        flash.write(PLAUSIBILITY_CHECK_FLAGS_FILE_NAME, plausibilityCheckFlags.toJsonStr());
    }
    rawValuesChanged = false;
    plausibilityCheckFlagsChanged = false;
    startWaiting();
}

void startWatingCalibration(void *configurator)
{
    Configurator *configurator_;
    configurator_ = (Configurator *)configurator;
    configurator_->startWaiting();
}