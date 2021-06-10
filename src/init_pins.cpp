#include "M5Stack.h"
#include "init_pins.hpp"

void initPins()
{
    pinMode(STEPPER_POWER_PIN, OUTPUT);
    pinMode(STEPPER_OUTPUT_PIN_1, OUTPUT);
    pinMode(STEPPER_OUTPUT_PIN_2, OUTPUT);
    pinMode(STEPPER_OUTPUT_PIN_3, OUTPUT);
    pinMode(STEPPER_OUTPUT_PIN_4, OUTPUT);
}
