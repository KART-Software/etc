#ifndef _SERIAL_LOGGER_H_
#define _SERIAL_LOGGER_H_

#define SERIAL_SPEED 28800

#include <Arduino.h>

class SerialLogger
{
public:
    SerialLogger();
    void initialize();
    void log(unsigned int cycleTime, double apps1Value, double apps2Value, double tps1Value, double tps2Value, bool isPlausible, String errStr);
};

#endif
