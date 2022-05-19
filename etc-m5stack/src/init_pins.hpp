#include <M5Stack.h>
#ifndef _INIT_PINS_H_
#define _INIT_PINS_H_

#define ARDUINO_A0 0
#define ARDUINO_A1 1
#define ARDUINO_A2 2
#define ARDUINO_A3 3
#define ARDUINO_A4 4

#define APPS_1_PIN ARDUINO_A0
#define APPS_2_PIN ARDUINO_A1
#define TPS_1_PIN ARDUINO_A2
#define TPS_2_PIN ARDUINO_A3
#define ITTR_PIN ARDUINO_A4 // IST Throttle Position Target Receiver (ITTR)

#define STEPPER_DIR_PIN 25
#define STEPPER_STEP_PIN 26
#define STEPPER_ENABLE_PIN 19

#define STEPPER_UP_BUTTON M5.BtnA
#define STEPPER_DOWN_BUTTON M5.BtnB
#define ADJUSTER_MODE_FINISH_BUTTON M5.BtnC
#define ENTER_ADJUSTER_MODE_BUTTON M5.BtnA
#define BUTTON_LONG_PRESS_THRESHOLD 2000

void initPins();

#endif