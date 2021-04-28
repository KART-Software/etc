#include "stepper_controller.hpp"

StepperController::
    StepperController(Apps *apps1, Apps *apps2, Tps *tps1, Tps *tps2)
    : stepper(
          Stepper(
              STEPPER_STEPS,
              STEPPER_OUTPUT_PIN_1,
              STEPPER_OUTPUT_PIN_2,
              STEPPER_OUTPUT_PIN_3,
              STEPPER_OUTPUT_PIN_4)),
      apps1(apps1), apps2(apps2), tps1(tps1), tps2(tps2)

{
    stepper.setSpeed(30);
}

void StepperController::control()
{
    int diff = (apps1->validatedConvertedValue() +
                apps2->validatedConvertedValue() -
                tps1->validatedConvertedValue() -
                tps2->validatedConvertedValue()) /
               2;
    stepper.step(diff); //TODO map
}

void StepperController::setOutputLimit()
{
}

int StepperController::validateOutput()
{
}

void StepperController::setStepperOn()
{
    digitalWrite(STEPPER_POWER_PIN, HIGH);
}
void StepperController::setStepperOff()
{
    digitalWrite(STEPPER_POWER_PIN, LOW);
}