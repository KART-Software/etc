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
      input(input),
      target(target)

{
    stepper.setSpeed(30);
}

void StepperController::control()
{
    stepper.step(*input);
}

void StepperController::setOutputLimit()
{
}

int StepperController::validateOutput()
{
}
