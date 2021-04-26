#include <Arduino.h>
#include <mcp_can.h>
#include <SPI.h>
#include <Stepper.h>

#include "error_handler.hpp"
#include "init_pins.hpp"

//Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

void setup()
{
  initPins();
}

void loop()
{
}