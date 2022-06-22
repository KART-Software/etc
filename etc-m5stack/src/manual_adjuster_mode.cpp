#include "manual_adjuster_mode.hpp"

void startManualAdjusterMode(void *servoController_)
{
    ServoController *servoController;
    servoController = (ServoController *)servoController_;
    while (true)
    {
        SERVO_UP_BUTTON.read();
        SERVO_DOWN_BUTTON.read();
        if (SERVO_UP_BUTTON.wasPressed())
        {
            servoController->rotate(1 * SERVO_DIRECTION);
        }
        if (SERVO_DOWN_BUTTON.wasPressed())
        {
            servoController->rotate(-1 * SERVO_DIRECTION);
        }
        delay(5);
    }
}