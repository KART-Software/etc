#include "pid.hpp"

PID::PID(double kp, double ki, double kd)
    : kp(kp), ki(ki), kd(kd) {}

// void PID::setTargetPosition(double targetPosition){
//     targetPosition = targetPosition;
// }

// void PID::setCurrentPosition(double currentPosition){
//     currentPosition = currentPosition;
// }

void PID::initialize(double targetPosition, double currentPosition)
{
    error = currentPosition - targetPosition;
    integral = 0;
}

double PID::calculate(double targetPosition, double currentPosition)
{
    errorPrev = error;
    error = currentPosition - targetPosition;
    integral = integral + error;
    differential = error - errorPrev;
    return constrain(-kp * error - ki * integral - kd * differential, minOutput, maxOutput);
}

void PID::setOutputLimits(double minOutput, double maxOutput)
{
    this->minOutput = minOutput;
    this->maxOutput = maxOutput;
}