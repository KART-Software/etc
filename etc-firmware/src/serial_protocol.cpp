#include "serial_protocol.hpp"

void SerialProtocol::initialize()
{
    Serial.begin(SERIAL_SPEED);
    while (!Serial && millis() < 3000)
    {
        // USB Serial 接続待ち（最大3秒）
    }
}

void SerialProtocol::sendSensorData(Apps &apps1, Apps &apps2, Ittr &ittr, Tps &tps1, Tps &tps2, Bps &bps,
                                    Target &target, bool isValid, ErrorHandler &errorHandler)
{
    StaticJsonDocument<SENSOR_MSG_JSON_SIZE> doc;
    doc["t"] = "s";
    doc["ts"] = millis();
    doc["a1r"] = apps1.getRawValue();
    doc["a2r"] = apps2.getRawValue();
    doc["a1"] = apps1.convertedValue();
    doc["a2"] = apps2.convertedValue();
    doc["ir"] = ittr.getRawValue();
    doc["i"] = ittr.convertedValue();
    doc["t1r"] = tps1.getRawValue();
    doc["t2r"] = tps2.getRawValue();
    doc["t1"] = tps1.convertedValue();
    doc["t2"] = tps2.convertedValue();
    doc["br"] = bps.getRawValue();
    doc["b"] = bps.convertedValue();
    doc["tgt"] = target.getTarget();
    doc["m"] = target.getModeString();
    doc["manual"] = target.isManual();
    doc["tgt_ittr"] = target.isIttr();
    doc["v"] = isValid;

    JsonArray err = doc.createNestedArray("err");
    uint8_t len = errorHandler.errorsLength();
    for (uint8_t j = 0; j < len; j++)
    {
        if (errorHandler.errors[j].raised)
        {
            err.add(errorHandler.errors[j].ID);
        }
    }

    serializeJson(doc, Serial);
    Serial.println();
}

void SerialProtocol::sendDebugf(const char *fmt, ...)
{
    char buf[192];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    StaticJsonDocument<DEBUG_MSG_JSON_SIZE> doc;
    doc["t"] = "d";
    doc["ts"] = millis();
    doc["msg"] = buf;
    serializeJson(doc, Serial);
    Serial.println();
}

void SerialProtocol::sendResponse(uint32_t id, bool ok)
{
    StaticJsonDocument<RESPONSE_MSG_JSON_SIZE> doc;
    doc["t"] = "r";
    doc["id"] = id;
    doc["ok"] = ok;
    serializeJson(doc, Serial);
    Serial.println();
}

void SerialProtocol::sendResponse(uint32_t id, bool ok, const JsonObject &data)
{
    StaticJsonDocument<RESPONSE_MSG_JSON_SIZE> doc;
    doc["t"] = "r";
    doc["id"] = id;
    doc["ok"] = ok;
    doc["data"] = data;
    serializeJson(doc, Serial);
    Serial.println();
}

bool SerialProtocol::readCommand(StaticJsonDocument<COMMAND_JSON_SIZE> &doc)
{
    if (!Serial.available())
    {
        return false;
    }

    String line = Serial.readStringUntil('\n');
    line.trim();
    if (line.length() == 0)
    {
        return false;
    }

    DeserializationError error = deserializeJson(doc, line);
    if (error)
    {
        return false;
    }

    return doc.containsKey("c");
}
