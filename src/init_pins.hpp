#include <M5Stack.h>
#ifndef _INIT_PINS_H_
#define _INIT_PINS_H_

#define ARDUINO_A0 0
#define ARDUINO_A1 1
#define ARDUINO_A2 2
#define ARDUINO_A3 3

#define APPS_1_PIN ARDUINO_A0
#define APPS_2_PIN ARDUINO_A1
#define TPS_1_PIN ARDUINO_A2
#define TPS_2_PIN ARDUINO_A3

#define STEPPER_ENABLE_PIN 19

// #define STEPPER_OUTPUT_PIN_1 16
// #define STEPPER_OUTPUT_PIN_2 17
#define STEPPER_DIR_PIN 25
#define STEPPER_STEP_PIN 26
#define STEPPER_ENABLE_PIN 1

void initPins();

#endif