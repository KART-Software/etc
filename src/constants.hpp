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
#define BPS_CH 1  // Brake Pressure Sensor
#endif

#ifdef ADS8688
#define ADC_NUM_CH 7
#define APPS_1_CH 6
#define APPS_2_CH 7
#define TPS_1_CH 0
#define TPS_2_CH 1
#define ITTR_CH 2          // IST Throttle Position Target Receiver (ITTR)
#define BPS_CH 3           // Brake Pressure Sensor
#define MOTOR_CURRENT_CH 5 // Connected to motor driver's CS PIN.The voltage is about 20 mV/A plus a 50 mV offset.
#define ADC_CHANNELS        \
    {                       \
        0, 1, 2, 3, 5, 6, 7 \
    } // Sorted Channels
#endif

////////////////////
/// PWM Settings ///
////////////////////

// #define HILITAND
// #define VNH5019
#define G2_18V17

#ifdef HILITAND
#define DC_MOTOR_ENABLE_PIN 5 // TODO CHANGE
#define DC_MOTOR_PWM_1_PIN 33
#define DC_MOTOR_PWM_2_PIN 32
#define PWM_1_LEDC_CHANNEL 0
#define PWM_2_LEDC_CHANNEL 1
#endif

#ifdef VNH5019
#define DC_MOTOR_IN_A_PIN 17
#define DC_MOTOR_IN_B_PIN 2
#define DC_MOTOR_EN_A_PIN 16
#define DC_MOTOR_EN_B_PIN 0
#define DC_MOTOR_PWM_PIN 4
#define PWM_LEDC_CHANNEL 0
#endif

#ifdef G2_18V17
#define DC_MOTOR_SLP_PIN 4
#define DC_MOTOR_PWM_PIN 16
#define DC_MOTOR_DIR_PIN 17
#define DC_MOTOR_FLT_PIN 15 // TODO CHANGE
#define PWM_LEDC_CHANNEL 0
#endif

//////////////////////////////
/// Motor Control Settings ///
//////////////////////////////

#define MOTOR_CONTROLL_CYCLE_TIME 1 // ms

#define KP 3
#define KI 0
#define KD 0.0

#define MOTOR_DIRECTION -1

///////////////////////
/// Sensor Settings ///
///////////////////////

#define TPS_1_RAW_MIN 18282
#define TPS_2_RAW_MIN 47580

#define TPS_1_RAW_MAX 7197
#define TPS_2_RAW_MAX 58558

#define TPS_MIN 0
#define TPS_MAX 100
#define TPS_MARGIN 15

// #define APPS_1_RAW_MIN 30720
// #define APPS_2_RAW_MIN 35154

// #define APPS_1_RAW_MAX 32907
// #define APPS_2_RAW_MAX 32977

#define APPS_1_RAW_MIN 65385
#define APPS_2_RAW_MIN 108

#define APPS_1_RAW_MAX 155
#define APPS_2_RAW_MAX 65467

#define APPS_MIN 0
#define APPS_MAX 100
#define APPS_MARGIN 50

#define ITTR_RAW_MAX 1023
#define ITTR_RAW_MIN 1

#define ITTR_MAX 100
#define ITTR_MIN 0

#define ITTR_MARGIN 0

#define BPS_RAW_MAX 4.5 * 65535 / 5.12 // 4.5V
#define BPS_RAW_MIN 0.5 * 65535 / 5.12 // 0.5V

#define BPS_MAX 1000                    // psi
#define BPS_MIN 0                       // psi
#define BPS_HIGH_PRESSURE_THRESHOLD 600 // psi
#define BPS_MARGIN 0

///////////////////////
/// Button Settings ///
///////////////////////

#define BUTTON_1_PIN 12
#define BUTTON_2_PIN 13
#define BUTTON_3_PIN 14

/////////////////////////////////
/// Other Output Pin Settings ///
/////////////////////////////////

#define FUEL_PUMP_PIN 33

#endif