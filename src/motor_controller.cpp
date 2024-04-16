#include "motor_controller.hpp"

MotorController::MotorController(TargetSensor &targetSensor, Tps &tps) : targetSensor(targetSensor), tps(tps)
{
}

void MotorController::initialize()
{
    dcMotor.initialize();
}

void MotorController::cycle()
{
    targetSensor.read();
    tps.read();
    double target = targetSensor.convertToTargetTp();
    double tp = tps.convertedValue();
    output = pid.compute(target, tp);
    dcMotor.write(output);
}

void MotorController::start()
{
    while (true)
    {
        cycle();
        delay(cycleTime);
    }
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

void startMotor(void *motorController)
{
    MotorController *controller;
    controller = (MotorController *)motorController;
    controller->start();
}