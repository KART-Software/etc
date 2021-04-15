#include "Arduino.h"
#include "init_pins.hpp"

void initPins()
{
    pinMode(TPS_PIN, INPUT);
    pinMode(APPS_PIN, INPUT);
}
