#ifndef _MOVING_AVERAGE_H_
#define _MOVING_AVERAGE_H_

#include <Arduino.h>

class MovingAverage
{
public:
    MovingAverage(uint size = 100);
    void add(float value);
    float getAvg();

private:
    const uint size;
    float *value;
    uint index = 0;
    bool sizeFull = false;
    float getAvg(uint size);
};

#endif