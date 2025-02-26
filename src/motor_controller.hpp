#ifndef _MOTOR_CONTROLLER_H_
#define _MOTOR_CONTROLLER_H_

#include "dcmotor.hpp"
#include "pid.hpp"
#include "sensors.hpp"

class MotorController
{
public:
    MotorController(Target &target, Tps &tps);
    void initialize();
    void cycle();
    void start();
    void setMotorOn();
    void setMotorOff();
    bool isOn();

private:
    DcMotor dcMotor = DcMotor();
    PID pid = PID();
    Target &target;
    Tps &tps;
    const unsigned long cycleTime = MOTOR_CONTROLL_CYCLE_TIME;
    double output;
};

void startMotor(void *motorController);

#endif