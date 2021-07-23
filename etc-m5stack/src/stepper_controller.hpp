#ifndef _STEPPER_CONTROLLER_H_
#define _STEPPER_CONTROLLER_H_

#include <Stepper.h>
#include <DRV8834.h>
#include <M5Stack.h>
#include <PID_v1.h>

#include "init_pins.hpp"
#include "sensors.hpp"

#define STEPPER_STEPS 200 //TODO change
#define MICROSTEP 1

#define STEPPER_OUTPUT_MAX 100 //TODO change
#define STEPPER_OUTPUT_MIN -100

#define STEPPER_CYCLE_TIME 0.1

#define PID_KP 0.5
#define PID_KI 0.0
#define PID_KD 0.0

#define INITIALIZE_RPM 30
#define INITIALIZE_STEPS 30
#define SAME_POSISTION_THRESHOLD 10

#define ENABLE disable
#define DISABLE enable

class StepperController
{
public:
    StepperController(Apps *apps1, Apps *apps2, Tps *tps1, Tps *tps2);
    void initializeOrigin();
    void control();
    void setStepperOn();
    void setStepperOff();
    void start();

private:
    DRV8834 drv8834;
    // Stepper stepper;
    Apps *apps1, *apps2;
    Tps *tps1, *tps2;
    PID pid;
    double tp, output, app;
    void setOutputLimit();
    int validateOutput();
};

void startStepper(void *stepperController);

#endif