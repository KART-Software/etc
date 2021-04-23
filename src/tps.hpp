/*
Throttle Potentio Meter

*/

#ifndef _TPS_H_
#define _TPS_H_

#include <Arduino.h>

#include "init_pins.hpp"
#include "globals.hpp"

#define TPS_SLOPE_1 1.3
#define TPS_SLOPE_2 1.3

#define TPS_INTERCEPT_1 10.0
#define TPS_INTERCEPT_2 10.0

#define TPS_MAX_1 150
#define TPS_MAX_2 150

#define TPS_MIN_1 150
#define TPS_MIN_2 150

double readTps1();
double readTps2();

#endif