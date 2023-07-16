#include "pid.hpp"

PID::PID(double kP, double kI, double kD, int8_t direction) : kP(kP), kI(kI), kD(kD)
{
    setDirection(direction);
}

void PID::setDirection(int8_t direction)
{
    if (direction > 0)
    {
        this->direction = 1;
    }
    else
    {
        this->direction = -1;
    }
}

double PID::compute(double setPoint, double position)
{
    unsigned long now = micros();
    double error = position - setPoint;
    double differential;

    if (lastTime == 0)
    {
        differential = 0.0;
    }
    else
    {
        double timeDelta = (now - lastTime) / 1000000.0; // sec
        differential = (error - lastError) / timeDelta;
        errorSum += error * timeDelta;
    }
    lastTime = now;
    lastError = error;

    return -direction * (kP * error + kI * errorSum + kD * differential);
}