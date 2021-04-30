#include <Arduino.h>
#include <mcp_can.h>
#include <SPI.h>
#include <Stepper.h>

#include "error_handler.hpp"
#include "init_pins.hpp"
#include "sensors.hpp"
#include "plausibility_validator.hpp"
#include "stepper_controller.hpp"

Apps apps1(APPS_1_MIN, APPS_1_MAX, APPS_1_INTERCEPT, APPS_1_SLOPE, APPS_1_PIN);
Apps apps2(APPS_2_MIN, APPS_2_MAX, APPS_2_INTERCEPT, APPS_2_SLOPE, APPS_2_PIN);
Tps tps1(TPS_1_MIN, TPS_1_MAX, TPS_1_INTERCEPT, TPS_1_SLOPE, TPS_1_PIN);
Tps tps2(TPS_2_MIN, TPS_2_MAX, TPS_2_INTERCEPT, TPS_2_SLOPE, TPS_2_PIN);
PlausibilityValidator plausibilityValidator(&apps1, &apps2, &tps1, &tps2);
StepperController stepperController(&apps1, &apps2, &tps1, &tps2);

bool isValid;

void setup()
{
  initPins();
  stepperController.setStepperOn();
}

void loop()
{
  apps1.read();
  apps2.read();
  tps1.read();
  tps2.read();

  if (plausibilityValidator.isCurrentlyValid())
  {
    stepperController.setStepperOff();
  }
  if (plausibilityValidator.isValid())
  {
    stepperController.control();
  }
}