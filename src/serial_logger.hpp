#ifndef _SERIAL_LOGGER_H_
#define _SERIAL_LOGGER_H_

#include <Arduino.h>

class SerialLogger
{
public:
    SerialLogger();
    void log(double apps1Value, double apps2Value, double tps1Value, double tps2Value, bool isPlausible, String errStr);
};

#endif
