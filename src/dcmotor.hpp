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
    DcMotor(uint8_t pwmPin1 = DC_MOTOR_PWM_1_PIN, uint8_t pwmPin2 = DC_MOTOR_PWM_2_PIN, uint8_t ch1 = PWM_1_LEDC_CHANNEL, uint8_t ch2 = PWM_2_LEDC_CHANNEL);
    void initialize();
    void setScaleMax(double scaleMax);
    void setOutputMax(uint16_t outputMax);
    void write(double value);
    void on();
    void off();

private:
    uint8_t pwmPin1, pwmPin2;
    uint8_t ch1, ch2;
    double scaleMax = DC_MOTOR_OUTPUT_SCALE_MAX;
    uint16_t outputMax = PWM_DUTY_MAX;
    double min(double a, double b);
    void write(uint16_t duty1, uint16_t duty2);
    void ledcAttach();
    void ledcDetach();
    bool isOn = false;
};

#endif