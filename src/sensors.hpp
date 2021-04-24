#ifndef _SENSORS_H_
#define _SENSORS_H_

#include <Arduino.h>

#include "init_pins.hpp"
#include "globals.hpp"

#define TPS_1_SLOPE 1.3
#define TPS_2_SLOPE 1.3

#define TPS_1_INTERCEPT 10.0
#define TPS_2_INTERCEPT 10.0

#define TPS_1_MAX 150
#define TPS_2_MAX 150

#define TPS_1_MIN 150
#define TPS_2_MIN 150

#define APPS_1_SLOPE 1.3
#define APPS_2_SLOPE 1.3

#define APPS_1_INTERCEPT 10.0
#define APPS_2_INTERCEPT 10.0

#define APPS_1_MAX 150
#define APPS_2_MAX 150

#define APPS_1_MIN 150
#define APPS_2_MIN 150

double readTps1();
double readTps2();

double readApps1();
double readApps2();

#endif