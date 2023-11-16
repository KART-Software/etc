#include "calibrator.hpp"

RawSensorValues::RawSensorValues() {}

RawSensorValues::RawSensorValues(uint16_t apps1Min, uint16_t apps1Max, uint16_t apps2Min, uint16_t apps2Max, uint16_t ittrMin, uint16_t ittrMax, uint16_t tps1Min, uint16_t tps1Max, uint16_t tps2Min, uint16_t tps2Max)
    : apps1Min(apps1Min), apps1Max(apps1Max), apps2Min(apps2Min), apps2Max(apps2Max), ittrMin(ittrMin), ittrMax(ittrMax), tps1Min(tps1Min), tps1Max(tps1Max), tps2Min(tps2Min), tps2Max(tps2Max) {}

void RawSensorValues::initialize()
{
    flash.initialize();
}

void RawSensorValues::saveToFlash()
{
    flash.write(toJsonStr());
}

void RawSensorValues::loadFromFlash()
{
    const char *jsonStr = flash.read();
    loadFronJsonStr(jsonStr);
}

void RawSensorValues::loadFronJsonStr(const char *jsonStr)
{
    StaticJsonDocument<JSON_SIZE> json;
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
    if (!ok)
    {
        // メンバーが足りなかったときはConstantsから読み込む
        loadFronConstants();
        saveToFlash();
    }
    else
    {
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
    }
}

void RawSensorValues::loadFronConstants()
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
}

const char *RawSensorValues::toJsonStr()
{
    StaticJsonDocument<JSON_SIZE> json;
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
    char *jsonStr = new char[JSON_SIZE];
    serializeJson(json, jsonStr, JSON_SIZE);
    return jsonStr;
}

Calibrator::Calibrator(Apps &apps1, Apps &apps2, Tps &tps1, Tps &tps2, Ittr &ittr, MotorController &motorController)
    : apps1(apps1), apps2(apps2), tps1(tps1), tps2(tps2), ittr(ittr), motorController(motorController)
{
}

void Calibrator::initialize()
{
    rawValues.initialize();
}

void Calibrator::calibrate()
{
    apps1.setRawMin(rawValues.apps1Min);
    apps1.setRawMax(rawValues.apps1Max);
    apps2.setRawMin(rawValues.apps2Min);
    apps2.setRawMax(rawValues.apps2Max);
    ittr.setRawMin(rawValues.ittrMin);
    ittr.setRawMax(rawValues.ittrMax);
    tps1.setRawMin(rawValues.tps1Min);
    tps1.setRawMax(rawValues.tps1Max);
    tps2.setRawMin(rawValues.tps2Min);
    tps2.setRawMax(rawValues.tps2Max);
}

void Calibrator::calibrateFromFlash()
{
    rawValues.loadFromFlash();
    calibrate();
}

void Calibrator::calibrate(char c)
{
    switch (c)
    {
    case APPS_MIN_CALIBRATE_FLAG:
        setAppsMin();
        Serial.println("---- APPS Min Set ----");
        break;
    case APPS_MAX_CALIBRATE_FLAG:
        setAppsMax();
        Serial.println("---- APPS Max Set ----");
        break;
    case TPS_MIN_CALIBRATE_FLAG:
        setTpsMin();
        Serial.println("---- TPS Min Set ----");
        break;
    case TPS_MAX_CALIBRATE_FLAG:
        setTpsMax();
        Serial.println("---- TPS Max Set ----");
        break;
    case CALIBRATION_FINISH_FLAG:
        Serial.println("---- Calibration Finish ----");
        finish();
        break;
    default:
        break;
    }
}

void Calibrator::setAppsMin()
{
    rawValues.apps1Min = apps1.setCurrentValRawMin();
    rawValues.apps2Min = apps2.setCurrentValRawMin();
    rawValues.ittrMin = ittr.setCurrentValRawMin();
}

void Calibrator::setAppsMax()
{
    rawValues.apps1Max = apps1.setCurrentValRawMax();
    rawValues.apps2Max = apps2.setCurrentValRawMax();
    rawValues.ittrMax = ittr.setCurrentValRawMax();
}

void Calibrator::setTpsMin()
{
    rawValues.tps1Min = tps1.setCurrentValRawMin();
    rawValues.tps2Min = tps2.setCurrentValRawMin();
}

void Calibrator::setTpsMax()
{
    rawValues.tps1Max = tps1.setCurrentValRawMax();
    rawValues.tps2Max = tps2.setCurrentValRawMax();
}

void Calibrator::start()
{
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

void Calibrator::startWaiting()
{
    while (true)
    {
        if (Serial.available())
        {
            if (Serial.read() == CALIBRATION_START_FLAG)
            {
                while (Serial.available())
                {
                    Serial.read();
                }
                Serial.println("---- Calibration Start ----");
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

void Calibrator::finish()
{
    rawValues.saveToFlash();
    startWaiting();
}

void startWatingCalibration(void *calibrator)
{
    Calibrator *tor;
    tor = (Calibrator *)calibrator;
    tor->startWaiting();
}