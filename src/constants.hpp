#include <Arduino.h>
#ifndef _INIT_PINS_H_
#define _INIT_PINS_H_

#define APPS_1_CH 0
#define APPS_2_CH 1
#define TPS_1_CH 2
#define TPS_2_CH 3
#define ITTR_CH 4 // IST Throttle Position Target Receiver (ITTR)

#define DC_MOTOR_PWM_1_PIN 25
#define DC_MOTOR_PWM_2_PIN 26

#define KP 5
#define KI 0
#define KD 0

#define MOTOR_DIRECTION -1

void initPins();

#endif