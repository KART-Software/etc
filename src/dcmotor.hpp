#ifndef _DC_MOTOR_H_
#define _DC_MOTOR_H_

#include <Arduino.h>
#include "constants.hpp"

#ifdef HILITAND
#define PWM_LEDC_FREQUENCY 78125
#define PWM_LEDC_RESOLUTION 10
#define DC_MOTOR_OUTPUT_SCALE_MAX 100.0
#define PWM_DUTY_MAX 1024
#endif

#ifdef VNH5019
#define PWM_LEDC_FREQUENCY 19531.25
#define PWM_LEDC_RESOLUTION 12
#define DC_MOTOR_OUTPUT_SCALE_MAX 100.0
#define PWM_DUTY_MAX 4096
#endif

#ifdef G2_18V17
#define PWM_LEDC_FREQUENCY 78125
#define PWM_LEDC_RESOLUTION 10
#define DC_MOTOR_OUTPUT_SCALE_MAX 100.0
#define PWM_DUTY_MAX 1024
#endif

#ifdef DRV8256P
#define PWM_LEDC_FREQUENCY 78125
#define PWM_LEDC_RESOLUTION 10
#define DC_MOTOR_OUTPUT_SCALE_MAX 100.0
#define PWM_DUTY_MAX 1024
#endif

#ifdef TB67H450
#define PWM_LEDC_FREQUENCY 78125
#define PWM_LEDC_RESOLUTION 10
#define DC_MOTOR_OUTPUT_SCALE_MAX 100.0
#define PWM_DUTY_MAX 1024
#endif

class DcMotor
{
public:
    DcMotor();
    void initialize();
    void write(double value);
    void on();
    void off();
    bool isOn();

private:
#ifdef HILITAND
    const uint8_t enablePin = DC_MOTOR_ENABLE_PIN;
    const uint8_t pwmPin1 = DC_MOTOR_PWM_1_PIN;
    const uint8_t pwmPin2 = DC_MOTOR_PWM_2_PIN;
    const uint8_t ch1 = PWM_1_LEDC_CHANNEL;
    const uint8_t ch2 = PWM_2_LEDC_CHANNEL;
    void write(uint16_t duty1, uint16_t duty2);
#endif
#ifdef VNH5019
    const uint8_t inAPin = DC_MOTOR_IN_A_PIN;
    const uint8_t inBPin = DC_MOTOR_IN_B_PIN;
    const uint8_t enAPin = DC_MOTOR_EN_A_PIN;
    const uint8_t enBPin = DC_MOTOR_EN_B_PIN;
    const uint8_t pwmPin = DC_MOTOR_PWM_PIN;
    const uint8_t ch = PWM_LEDC_CHANNEL;
    void write(uint16_t duty, uint8_t inAState, uint8_t inBState);
#endif
#ifdef G2_18V17
    const uint8_t slpPin = DC_MOTOR_SLP_PIN;
    const uint8_t pwmPin = DC_MOTOR_PWM_PIN;
    const uint8_t dirPin = DC_MOTOR_DIR_PIN;
    const uint8_t ch = 0;
    const uint8_t relayPin = DC_MOTOR_RELAY_PIN;
    void write(uint16_t duty, uint8_t dir);
#endif
#ifdef DRV8256P
    const uint8_t slpPin = DC_MOTOR_SLP_PIN;
    const uint8_t pwmPin1 = DC_MOTOR_PWM_1_PIN;
    const uint8_t pwmPin2 = DC_MOTOR_PWM_2_PIN;
    const uint8_t ch1 = PWM_1_LEDC_CHANNEL;
    const uint8_t ch2 = PWM_2_LEDC_CHANNEL;
    const uint8_t relayPin = DC_MOTOR_RELAY_PIN;
    void write(uint16_t duty1, uint16_t duty2);
#endif
#ifdef TB67H450
    const uint8_t pwmPin1 = DC_MOTOR_IN_1_PIN;
    const uint8_t pwmPin2 = DC_MOTOR_IN_2_PIN;
    const uint8_t ch1 = PWM_1_LEDC_CHANNEL;
    const uint8_t ch2 = PWM_2_LEDC_CHANNEL;
    const uint8_t relayPin = DC_MOTOR_RELAY_PIN;
    void write(uint16_t duty1, uint16_t duty2);
#endif
    const double scaleMax = DC_MOTOR_OUTPUT_SCALE_MAX;
    const uint16_t outputMax = PWM_DUTY_MAX;
    double min(double a, double b);
    void ledcAttach();
    void ledcDetach();
    bool _isOn = false;
};

#endif