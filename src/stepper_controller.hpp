#ifndef _STEPPER_CONTROLLER_H_
#define _STEPPER_CONTROLLER_H_

#include <Stepper.h>
#include <PID_v1.h>
#include <Arduino.h>

#include "init_pins.hpp"

#define STEPPER_STEPS 100 //TODO change
#define PID_KP 2.0
#define PID_KI 5.0
#define PID_KD 1.0

#define STEPPER_OUTPUT_MAX 100 //TODO change
#define STEPPER_OUTPUT_MIN -100

class StepperController
{
public:
    StepperController(double *input, double *target);
    void control();

private:
    Stepper stepper;
    double *input;
    double *target;
    double *output;
    void setOutputLimit();
    int validateOutput();
};

#endif