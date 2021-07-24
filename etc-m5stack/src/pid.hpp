#ifndef _PID_H_
#define _PID_H_

#include <M5Stack.h>

class PID
{
public:
    PID(double kp, double ki, double kd);
    // void setTargetPosition(double targetPosition);
    // void setCurrentPosition(double currentPosition);
    void initialize(double targetPosition, double currentPosition);
    double calculate(double targetPosition, double currentPosition);
    void setOutputLimits(double minOutput, double maxOutput);

private:
    const double kp, ki, kd;
    double error, errorPrev, integral, differential;
    double minOutput, maxOutput;
    // double targetPosition, currentPosition;
};

#endif