#ifndef _PID_H_
#define _PID_H_

#include <Arduino.h>
#include "constants.hpp"

class PID
{
public:
    PID(double kP = KP, double kI = KI, double kD = KD, int8_t direction = MOTOR_DIRECTION);
    double compute(double setPoint, double position);
    void setDirection(int8_t direction);

private:
    double kP, kI, kD;
    double errorSum = 0.0;
    double lastError;
    unsigned long lastTime = 0;
    int8_t direction;
};

#endif