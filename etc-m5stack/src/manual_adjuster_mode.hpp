#ifndef _MANUAL_ADJUSTE_MODE_
#define _MANUAL_ADJUSTE_MODE_

#include "servo_controller.hpp"
#include "globals.hpp"
#include "lcd_indicator.hpp"
#include "init_pins.hpp"

void startManualAdjusterMode(ServoController *servoController);

void startInterface(void *interface);

#endif