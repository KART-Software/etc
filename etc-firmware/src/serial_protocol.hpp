#ifndef _SERIAL_PROTOCOL_H_
#define _SERIAL_PROTOCOL_H_

#include <Arduino.h>
#include <ArduinoJson.h>
#include "sensors.hpp"
#include "error_handler.hpp"

#define SENSOR_MSG_JSON_SIZE 512
#define DEBUG_MSG_JSON_SIZE 256
#define RESPONSE_MSG_JSON_SIZE 512
#define COMMAND_JSON_SIZE 512
#define SENSOR_SEND_INTERVAL 20 // ms (50Hz)

class SerialProtocol
{
public:
    // Send sensor data message
    static void sendSensorData(
        Apps &apps1, Apps &apps2, Ittr &ittr,
        Tps &tps1, Tps &tps2, Bps &bps,
        Target &target, bool isValid, ErrorHandler &errorHandler);

    // Send debug log message
    static void sendDebug(const char *msg);

    // Send command response
    static void sendResponse(uint32_t id, bool ok);
    static void sendResponse(uint32_t id, bool ok, const JsonObject &data);

    // Read and parse a command from serial. Returns true if a command was parsed.
    static bool readCommand(StaticJsonDocument<COMMAND_JSON_SIZE> &doc);
};

#endif
