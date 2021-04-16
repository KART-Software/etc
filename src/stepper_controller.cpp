#include "stepper_controller.hpp"

StepperController::
    StepperController(double *input, double *target)
    : stepper(
          Stepper(
              STEPPER_STEPS,
              STEPPER_OUTPUT_PIN_1,
              STEPPER_OUTPUT_PIN_2,
              STEPPER_OUTPUT_PIN_3,
              STEPPER_OUTPUT_PIN_4)),
      pid(
          PID(
              input,
              output,
              target,
              PID_KP,
              PID_KI,
              PID_KD,
              AUTOMATIC)),
      input(input),
      target(target)

{
    stepper.setSpeed(30);
    pid.SetOutputLimits(STEPPER_OUTPUT_MIN, STEPPER_OUTPUT_MAX);
}

void StepperController::control()
{
    pid.Compute();
    stepper.step(*input);
}

void StepperController::setOutputLimit()
{
}

int StepperController::validateOutput()
{
}
