#include <Arduino.h>
#include <mcp_can.h>
#include <SPI.h>
#include <Stepper.h>

#include "error_handler.hpp"
#include "init_pins.hpp"
#include "sensors.hpp"

//Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

Apps apps1(APPS_1_MIN, APPS_1_MAX, APPS_1_INTERCEPT, APPS_1_SLOPE, APPS_1_PIN);
Apps apps2(APPS_2_MIN, APPS_2_MAX, APPS_2_INTERCEPT, APPS_2_SLOPE, APPS_2_PIN);
Tps tps1(TPS_1_MIN, TPS_1_MAX, TPS_1_INTERCEPT, TPS_1_SLOPE, TPS_1_PIN);
Tps tps2(TPS_2_MIN, TPS_2_MAX, TPS_2_INTERCEPT, TPS_2_SLOPE, TPS_2_PIN);

void setup()
{
  initPins();
}

void loop()
{
}