#include "stepper_controller.hpp"

StepperController::
    StepperController(Apps *apps, Tps *tps)
    : drv8834(DRV8834(STEPPER_STEPS, STEPPER_DIR_PIN, STEPPER_STEP_PIN, STEPPER_ENABLE_PIN)),
      apps(apps), tps(tps),
      pid(PID_KP, PID_KI, PID_KD, STEPPER_CYCLE_TIME_MS)
{
    drv8834.setEnableActiveState(LOW);
    drv8834.setSpeedProfile(BasicStepperDriver::CONSTANT_SPEED);
    drv8834.setMicrostep(MICROSTEP);
    drv8834.begin();
    pid.setOutputLimits(STEPPER_OUTPUT_MIN, STEPPER_OUTPUT_MAX);
    pid.initialize(tps->convertedValue(), apps->convertedValue());
}

// void StepperController::initializeOrigin()
// {
//     drv8834.setRPM(INITIALIZE_RPM);
//     int16_t tps1RawMinValue, tps1RawMaxValue, tps2RawMinValue, tps2RawMaxValue;

//     tps1RawMinValue = gAdc.value[TPS_1_PIN];
//     while (true)
//     {
//         drv8834.move(-INITIALIZE_STEPS);
//         gAdc.read();
//         if (abs((int)gAdc.value[TPS_1_PIN] - (int)tps1RawMinValue) < SAME_POSISTION_THRESHOLD)
//         {
//             tps1RawMinValue = gAdc.value[TPS_1_PIN];
//             tps2RawMinValue = gAdc.value[TPS_2_PIN];
//             break;
//         }
//         tps1RawMinValue = gAdc.value[TPS_1_PIN];
//     }

//     tps1RawMaxValue = gAdc.value[TPS_1_PIN];
//     while (true)
//     {
//         drv8834.move(INITIALIZE_STEPS);
//         gAdc.read();
//         if (abs((int)gAdc.value[TPS_1_PIN] - (int)tps1RawMaxValue))
//         {
//             tps1RawMaxValue = gAdc.value[TPS_1_PIN];
//             tps2RawMaxValue = gAdc.value[TPS_2_PIN];
//             break;
//         }
//         tps1RawMaxValue = gAdc.value[TPS_1_PIN];
//     }
// }

void StepperController::control()
{
    // gAdc.read();
    // tps1->read();
    // apps1->read();
    tp = tps->convertedValue();
    app = apps->convertedValue();
    targetTp = apps->convertToTargetTp();
    output = pid.calculate(targetTp, tp);
    drv8834.setRPM(abs(output));
    drv8834.move(output * STEPPER_CYCLE_TIME);
}

void StepperController::setOutputLimit()
{
}

int StepperController::validateOutput()
{
}

void StepperController::start()
{
    while (1)
    {
        this->control();
        delay(1);
    }
}

void StepperController::setStepperOn()
{
    drv8834.enable();
    state = State::On;
}
void StepperController::setStepperOff()
{
    drv8834.disable();
    state = State::Off;
}
enum StepperController::State StepperController::getState()
{
    return state;
}

void StepperController::move(int distance)
{
    drv8834.move(distance);
}

void startStepper(void *stepperController)
{
    StepperController *controller;
    controller = (StepperController *)stepperController;
    controller->start();
}