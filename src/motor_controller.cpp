#include "motor_controller.hpp"

MotorController::MotorController(Target &target, Tps &tps) : target(target), tps(tps)
{
}

void MotorController::initialize()
{
    dcMotor.initialize();
}

void MotorController::cycle()
{
    double target_ = target.getTarget();
    double tp = tps.convertedValue();
    output = pid.compute(target_, tp);
    dcMotor.write(output);
}

void MotorController::setMotorOn()
{
    dcMotor.on();
}

void MotorController::setMotorOff()
{
    dcMotor.off();
}

bool MotorController::isOn()
{
    return dcMotor.isOn();
}