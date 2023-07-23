#ifndef _DC_MOTOR_H_
#define _DC_MOTOR_H_

#include <Arduino.h>
#include "constants.hpp"

#define PWM_LEDC_FREQUENCY 78125
#define PWM_LEDC_RESOLUTION 10
#define DC_MOTOR_OUTPUT_SCALE_MAX 100.0
#define PWM_DUTY_MAX 1024

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
    const uint8_t enablePin = DC_MOTOR_ENABLE_PIN;
    const uint8_t pwmPin1 = DC_MOTOR_PWM_1_PIN;
    const uint8_t pwmPin2 = DC_MOTOR_PWM_2_PIN;
    const uint8_t ch1 = PWM_1_LEDC_CHANNEL;
    const uint8_t ch2 = PWM_2_LEDC_CHANNEL;
    const double scaleMax = DC_MOTOR_OUTPUT_SCALE_MAX;
    const uint16_t outputMax = PWM_DUTY_MAX;
    double min(double a, double b);
    void write(uint16_t duty1, uint16_t duty2);
    void ledcAttach();
    void ledcDetach();
    bool _isOn = false;
};

#endif