#ifndef _STEPPER_CONTROLLER_H_
#define _STEPPER_CONTROLLER_H_

#include <DRV8834.h>
#include <Arduino.h>

#include "pid.hpp"
#include "init_pins.hpp"
#include "sensors.hpp"

#define STEPPER_STEPS 200 // TODO change
#define MICROSTEP 1

#define STEPPER_OUTPUT_MAX 800 // TODO change
#define STEPPER_OUTPUT_MIN -800

#define STEPPER_CYCLE_TIME 0.05
#define STEPPER_CYCLE_TIME_MS 50

#define PID_KP 15.0
#define PID_KI 0.0
#define PID_KD 0.0

#define INITIALIZE_RPM 30
#define INITIALIZE_STEPS 30
#define SAME_POSISTION_THRESHOLD 10

#define STEPPER_DIRECTION 1 // or -1

class StepperController
{
public:
    enum State
    {
        Off,
        On
    };
    StepperController(Apps *apps, Tps *tps);
    void initializeOrigin();
    void control();
    void setStepperOn();
    void setStepperOff();
    void start();
    void move(int distance);
    enum State getState();

private:
    DRV8834 drv8834;
    Apps *apps;
    Tps *tps;
    PID pid;
    double tp, output, app, targetTp;
    void setOutputLimit();
    int validateOutput();
    enum State state;
};

void startStepper(void *stepperController);

#endif