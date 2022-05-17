#ifndef _LCD_LOGGER_H_
#define _LCD_LOGGER_H_

#include <M5Stack.h>

class LcdLogger
{
public:
    LcdLogger();
    void initialize();
    void log(unsigned int cycleTime, double apps1Value, double apps2Value, double tps1Value, double tps2Value, bool isPlausible, String errStr);
};

#endif
