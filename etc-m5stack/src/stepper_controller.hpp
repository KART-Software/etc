#ifndef _STEPPER_CONTROLLER_H_
#define _STEPPER_CONTROLLER_H_

#include <DRV8834.h>
#include <M5Stack.h>

#include "pid.hpp"
#include "init_pins.hpp"
#include "sensors.hpp"

#define STEPPER_STEPS 200 //TODO change
#define MICROSTEP 1

#define STEPPER_OUTPUT_MAX 400 //TODO change
#define STEPPER_OUTPUT_MIN -400

#define STEPPER_CYCLE_TIME 0.1

#define PID_KP 10.0
#define PID_KI 0.0
#define PID_KD 0.0

#define INITIALIZE_RPM 30
#define INITIALIZE_STEPS 30
#define SAME_POSISTION_THRESHOLD 10

class StepperController
{
public:
    enum State
    {
        Off,
        On
    };
    StepperController(Apps *apps1, Apps *apps2, Tps *tps1, Tps *tps2);
    void initializeOrigin();
    void control();
    void setStepperOn();
    void setStepperOff();
    void start();
    enum State getState();

private:
    DRV8834 drv8834;
    Apps *apps1, *apps2;
    Tps *tps1, *tps2;
    PID pid;
    double tp, output, app;
    void setOutputLimit();
    int validateOutput();
    enum State state;
};

void startStepper(void *stepperController);

#endif