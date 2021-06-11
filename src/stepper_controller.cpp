#include "stepper_controller.hpp"

StepperController::
    StepperController(Apps *apps1, Apps *apps2, Tps *tps1, Tps *tps2)
    : stepper(
          Stepper(
              STEPPER_STEPS,
              STEPPER_OUTPUT_PIN_1,
              STEPPER_OUTPUT_PIN_2)),
      apps1(apps1), apps2(apps2), tps1(tps1), tps2(tps2),
      pid(&tp, &output, &app, PID_KP, PID_KI, PID_KD, DIRECT)

{
    pid.SetOutputLimits(STEPPER_OUTPUT_MIN, STEPPER_OUTPUT_MAX);
}

void StepperController::control()
{
    tp = tps1->convertedValue();
    app = apps1->convertedValue();
    pid.Compute();
    stepper.setSpeed(abs(output));
    stepper.step(output * STEPPER_CYCLE_TIME); //TODO map
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