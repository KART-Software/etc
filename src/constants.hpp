#ifndef _CONTANTS_H_
#define _CONSTANTS_H_

#include <Arduino.h>

//////////////////////
/// IST Controller ///
//////////////////////

// #define IST_CONTROLLER

////////////////////
/// ADC Settings ///
////////////////////

/// MCP3208 or ADS8688 ///
// #define MCP3208
#define ADS8688

#ifdef MCP3208
#define ADC_NUM_CH 6
#define APPS_1_CH 4
#define APPS_2_CH 5
#define TPS_1_CH 2
#define TPS_2_CH 3
#define ITTR_CH 0 // IST Throttle Position Target Receiver (ITTR)
#define BSE_CH 1  // Brake System Encoder
#endif

#ifdef ADS8688
#define ADC_NUM_CH 7       // TODO CHANGE
#define APPS_1_CH 0        // TODO CHANGE
#define APPS_2_CH 1        // TODO CHANGE
#define TPS_1_CH 2         // TODO CHANGE
#define TPS_2_CH 3         // TODO CHANGE
#define ITTR_CH 4          // TODO CHANGE  // IST Throttle Position Target Receiver (ITTR)
#define BSE_CH 5           // TODO CHANGE  // Brake System Encoder
#define MOTOR_CURRENT_CH 6 // TODO CHANGE  // Connected to motor driver's CS PIN.The voltage is about 20 mV/A plus a 50 mV offset.
#define ADC_CHANNELS        \
    {                       \
        0, 1, 2, 3, 4, 5, 6 \
    } // TODO CHANGE  // Sorted Channels
#endif

////////////////////
/// PWM Settings ///
////////////////////

#define DC_MOTOR_ENABLE_PIN 5 // TODO CHANGE

#define DC_MOTOR_PWM_1_PIN 33
#define DC_MOTOR_PWM_2_PIN 32
#define PWM_1_LEDC_CHANNEL 0
#define PWM_2_LEDC_CHANNEL 1

//////////////////////////////
/// Motor Control Settings ///
//////////////////////////////

#define MOTOR_CONTROLL_CYCLE_TIME 1 // ms

#define KP 3
#define KI 0
#define KD 0.01

#define MOTOR_DIRECTION -1

///////////////////////
/// Sensor Settings ///
///////////////////////

#define TPS_1_RAW_MIN 1817
#define TPS_2_RAW_MIN 2272

#define TPS_1_RAW_MAX 1120
#define TPS_2_RAW_MAX 2967

#define TPS_1_MIN 0
#define TPS_2_MIN 0

#define TPS_1_MAX 100
#define TPS_2_MAX 100

#define TPS_MARGIN 5

#define APPS_1_RAW_MIN 7
#define APPS_2_RAW_MIN 4066

#define APPS_1_RAW_MAX 4068
#define APPS_2_RAW_MAX 12

#define APPS_1_MIN 0
#define APPS_2_MIN 0

#define APPS_1_MAX 100
#define APPS_2_MAX 100

#define APPS_MARGIN 50

#define ITTR_RAW_MAX 1023
#define ITTR_RAW_MIN 1

#define ITTR_MAX 100
#define ITTR_MIN 0

#define ITTR_MARGIN 0

///////////////////////
/// Button Settings ///
///////////////////////

#define BUTTON_1_PIN 1 // TODO CHANGE
#define BUTTON_2_PIN 2 // TODO CHANGE
#define BUTTON_3_PIN 3 // TODO CHANGE

#define MONITOR_ON_PIN BUTTON_1_PIN

/////////////////////////////////
/// Other Output Pin Settings ///
/////////////////////////////////

#define FUEL_PUMP_PIN 4 // TODO CHANGE

#endif