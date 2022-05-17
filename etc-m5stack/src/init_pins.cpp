#include "init_pins.hpp"

void initPins()
{
    pinMode(STEPPER_ENABLE_PIN, OUTPUT);
    pinMode(STEPPER_DIR_PIN, OUTPUT);
    pinMode(STEPPER_STEP_PIN, OUTPUT);
}
