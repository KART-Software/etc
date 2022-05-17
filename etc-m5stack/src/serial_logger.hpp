#ifndef _SERIAL_LOGGER_H_
#define _SERIAL_LOGGER_H_

#define SERIAL_SPEED 115200

#include <M5Stack.h>

class SerialLogger
{
public:
    SerialLogger();
    void initialize();
    void log(unsigned int cycleTime, double apps1Value, double apps2Value, double tps1Value, double tps2Value, bool isPlausible, String errStr);
    void log(uint apps1RawValue, uint apps2RawValue, uint tps1RawValue, uint tps2RawValue);
};

#endif
