#include "motor_controller.hpp"

MotorController::MotorController(Apps &apps, Tps &tps) : apps(apps), tps(tps)
{
}

void MotorController::initialize()
{
    dcMotor.initialize();
}

void MotorController::cycle()
{
    apps.read();
    tps.read();
    double target = apps.convertToTargetTp();
    double tp = tps.convertedValue();
    output = pid.compute(target, tp);
    // Serial.print(target);
    // Serial.print(" ");
    // Serial.println(tp);
    dcMotor.write(output);
}

void MotorController::start()
{
    while (true)
    {
        cycle();
        delayMicroseconds(cycleTime);
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

void startMotor(void *motorController)
{
    MotorController *controller;
    controller = (MotorController *)motorController;
    controller->start();
}