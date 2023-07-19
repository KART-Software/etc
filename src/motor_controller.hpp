#ifndef _MOTOR_CONTROLLER_H_
#define _MOTOR_CONTROLLER_H_

#include "dcmotor.hpp"
#include "pid.hpp"
#include "sensors.hpp"

class MotorController
{
public:
    enum State
    {
        Off,
        On
    };
    MotorController(Apps &apps, Tps &tps, DcMotor dcMotor = DcMotor(), PID pid = PID(), unsigned long cycleTime = MOTOR_CONTROLL_CYCLE_TIME);
    void initialize();
    void cycle();
    void start();
    void setMotorOn();
    void setMotorOff();
    // void move(float tpDistance);
    enum State getState();

private:
    DcMotor dcMotor;
    PID pid;
    Apps &apps;
    Tps &tps;
    double output;
    unsigned long cycleTime;
    enum State state;
};

void startMotor(void *motorController);

#endif