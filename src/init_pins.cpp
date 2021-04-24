#include "Arduino.h"
#include "init_pins.hpp"

void initPins()
{
    pinMode(TPS_1_PIN, INPUT);
    pinMode(TPS_2_PIN, INPUT);
    pinMode(APPS_1_PIN, INPUT);
    pinMode(APPS_2_PIN, INPUT);
}
