#include "serial_logger.hpp"

SerialLogger::SerialLogger()
{
    // Serial.begin(SERIAL_SPEED);
}
void SerialLogger::initialize()
{
    Serial.begin(SERIAL_SPEED);
}

void SerialLogger::log(unsigned int cycleTime, double apps1Value, double apps2Value, double ittrValue, double tps1Value, double tps2Value, bool isPlausible, String errStr)
{
    String logStr = "Cycle: " + String(cycleTime) +
                    ", APPS1: " + String(apps1Value) +
                    ", APPS2: " + String(apps2Value) +
                    ", ITTR: " + String(ittrValue) +
                    ", TPS1: " + String(tps1Value) +
                    ", TPS2: " + String(tps2Value) +
                    ", Plausible: " + String(isPlausible) +
                    ", Error: " + errStr;
    Serial.println(logStr);
}

void SerialLogger::log(uint apps1RawValue, uint apps2RawValue, uint ittrRawValue, uint tps1RawValue, uint tps2RawValue)
{
    String logStr = "APPS1: " + String(apps1RawValue) +
                    ", APPS2: " + String(apps2RawValue) +
                    ", ITTR: " + String(ittrRawValue) +
                    ", TPS1: " + String(tps1RawValue) +
                    ", TPS2: " + String(tps2RawValue);
    Serial.println(logStr);
}