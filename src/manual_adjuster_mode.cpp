#include "manual_adjuster_mode.hpp"

void startManualAdjusterMode(void *stepperController_)
{
    StepperController *stepperController;
    stepperController = (StepperController *)stepperController_;
    while (true)
    {
        STEPPER_UP_BUTTON.read();
        STEPPER_DOWN_BUTTON.read();
        if (STEPPER_UP_BUTTON.wasPressed())
        {
            stepperController->move(10 * STEPPER_DIRECTION);
        }
        if (STEPPER_DOWN_BUTTON.wasPressed())
        {
            stepperController->move(-10 * STEPPER_DIRECTION);
        }
        delay(5);
    }
}