#include "serial_logger.hpp"

SerialLogger::SerialLogger()
{
    Serial.begin(28800);
}

void SerialLogger::log(double apps1Value, double apps2Value, double tps1Value, double tps2Value, bool isPlausible, String errStr)
{
    String logStr = "APPS1: " + String(apps1Value) +
                    ", APPS2: " + String(apps2Value) +
                    ", TPS1: " + String(tps1Value) +
                    ", TPS2: " + String(tps2Value) +
                    ", Plausible: " + String(isPlausible) +
                    ", Error: " + errStr;
    Serial.print(logStr);
}