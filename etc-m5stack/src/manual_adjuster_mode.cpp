#include "manual_adjuster_mode.hpp"

void startInterface(void *interface_)
{
    ManualParameterAdjusterInterface *interface;
    interface = (ManualParameterAdjusterInterface *)interface_;
    while (true)
    {
        interface->update();
        delay(ADJUSTER_INTERFACE_UPDATE_PER_MS);
    }
}

void startManualAdjusterMode(ServoController *servoController)
{
    TaskHandle_t interfaceTask;
    ManualParameterAdjusterInterface interface(servoController);
    xTaskCreatePinnedToCore(startInterface, "InterfaceTask", 8192, (void *)&interface, 1, &interfaceTask, 1);
    while (true)
    {
        SERVO_UP_BUTTON.read();
        SERVO_DOWN_BUTTON.read();
        ADJUSTER_MODE_FINISH_BUTTON.read();

        if (SERVO_UP_BUTTON.wasPressed())
        {
            servoController->rotate(1);
        }
        if (SERVO_DOWN_BUTTON.wasPressed())
        {
            servoController->rotate(-1);
        }
        if (ADJUSTER_MODE_FINISH_BUTTON.pressedFor(BUTTON_LONG_PRESS_THRESHOLD))
        {
            break;
        }
        delay(5);
    }
    vTaskDelete(interfaceTask);
}