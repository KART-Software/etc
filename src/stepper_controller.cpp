#include "stepper_controller.hpp"

StepperController::
    StepperController(Apps *apps1, Apps *apps2, Tps *tps1, Tps *tps2)
    : // stepper(
      //       Stepper(
      //           STEPPER_STEPS,
      //           STEPPER_OUTPUT_PIN_1,
      //           STEPPER_OUTPUT_PIN_2)),
      drv8834(DRV8834(STEPPER_STEPS, STEPPER_DIR_PIN, STEPPER_STEP_PIN)),
      apps1(apps1),
      apps2(apps2), tps1(tps1), tps2(tps2),
      pid(&tp, &output, &app, PID_KP, PID_KI, PID_KD, DIRECT)
{
    drv8834.setSpeedProfile(BasicStepperDriver::CONSTANT_SPEED);
    drv8834.setMicrostep(MICROSTEP);
    pid.SetOutputLimits(STEPPER_OUTPUT_MIN, STEPPER_OUTPUT_MAX);
    pid.SetMode(AUTOMATIC);
}

void StepperController::control()
{
    // gAdc.read();
    tps1->read();
    apps1->read();
    tp = tps1->convertedValue();
    app = apps1->convertedValue();
    pid.Compute();
    drv8834.setRPM(abs(output));
    drv8834.move(output * STEPPER_CYCLE_TIME); //TODO map
}

void StepperController::setOutputLimit()
{
}

int StepperController::validateOutput()
{
}

void StepperController::start()
{
    while (1)
    {
        this->control();
        delay(1);
    }
}

void StepperController::setStepperOn()
{
    digitalWrite(STEPPER_POWER_PIN, HIGH);
}
void StepperController::setStepperOff()
{
    digitalWrite(STEPPER_POWER_PIN, LOW);
}

void startStepper(void *stepperController)
{
    StepperController *controller;
    controller = (StepperController *)stepperController;
    controller->start();
}