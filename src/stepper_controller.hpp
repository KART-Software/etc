#ifndef _STEPPER_CONTROLLER_H_
#define _STEPPER_CONTROLLER_H_

#include <Stepper.h>
#include <PID_v1.h>
#include <Arduino.h>

#include "init_pins.hpp"
#include "sensors.hpp"

#define STEPPER_STEPS 100 //TODO change
#define PID_KP 2.0
#define PID_KI 5.0
#define PID_KD 1.0

#define STEPPER_OUTPUT_MAX 100 //TODO change
#define STEPPER_OUTPUT_MIN -100

class StepperController
{
public:
    StepperController(Apps *apps1, Apps *apps2, Tps *tps1, Tps *tps2);
    void control();
    void setStepperOn();
    void setStepperOff();

private:
    Stepper stepper;
    Apps *apps1, *apps2;
    Tps *tps1, *tps2;
    void setOutputLimit();
    int validateOutput();
};

#endif