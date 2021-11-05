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

#define SERVO_POWER_PIN 19
#define SERVO_PWM_PIN 26

#define SERVO_STOP_BUTTON M5.BtnA
#define CLEAR_ERROR_BUTTON M5.BtnB
#define ENTER_ADJUSTER_MODE_BUTTON M5.BtnC

#define BUTTON_LONG_PRESS_THRESHOLD 2000

void initPins();

#endif