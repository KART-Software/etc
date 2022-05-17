#include "lcd_logger.hpp"

LcdLogger::LcdLogger()
{
    // Serial.begin(SERIAL_SPEED);
}
void LcdLogger::initialize()
{
    M5.Lcd.begin();
}

void LcdLogger::log(unsigned int cycleTime, double apps1Value, double apps2Value, double tps1Value, double tps2Value, bool isPlausible, String errStr)
{
    String logStr = "Cycle: " + String(cycleTime) +
                    ", APPS1: " + String(apps1Value) +
                    ", APPS2: " + String(apps2Value) +
                    ", TPS1: " + String(tps1Value) +
                    ", TPS2: " + String(tps2Value) +
                    ", Plausible: " + String(isPlausible) +
                    ", Error: " + errStr;
    M5.Lcd.println(logStr);
}