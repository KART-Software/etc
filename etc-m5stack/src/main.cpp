#include <M5Stack.h>
// #include <mcp_can.h>
#include <SPI.h>
#include <Stepper.h>
#include <M5Stack.h>

#include "error_handler.hpp"
#include "init_pins.hpp"
#include "sensors.hpp"
#include "plausibility_validator.hpp"
#include "stepper_controller.hpp"
#include "serial_logger.hpp"

TaskHandle_t stepperControllTask;

Apps apps1(APPS_1_MIN, APPS_1_MAX, APPS_1_RAW_MIN, APPS_1_RAW_MAX, APPS_1_PIN);
Apps apps2(APPS_2_MIN, APPS_2_MAX, APPS_2_RAW_MIN, APPS_2_RAW_MAX, APPS_2_PIN);
Tps tps1(TPS_1_MIN, TPS_1_MAX, TPS_1_RAW_MIN, TPS_1_RAW_MAX, TPS_1_PIN);
Tps tps2(TPS_2_MIN, TPS_2_MAX, TPS_2_RAW_MIN, TPS_2_RAW_MAX, TPS_2_PIN);
PlausibilityValidator plausibilityValidator(&apps1, &apps2, &tps1, &tps2);
StepperController stepperController(&apps1, &apps2, &tps1, &tps2);
SerialLogger serialLogger;
unsigned long currentTime = 0;
unsigned long lastTime = 0;

void setup()
{
  M5.begin();
  serialLogger.initialize();
  initPins();
  stepperController.setStepperOn();
  gAdc.begin();
  xTaskCreatePinnedToCore(startStepper, "StepperConstrollTask", 8192, (void *)&stepperController, 1, &stepperControllTask, 1);
}

void loop()
{
  currentTime = millis();
  gAdc.read();
  apps1.read();
  apps2.read();
  tps1.read();
  tps2.read();

  if (!plausibilityValidator.isCurrentlyValid())
  {
    stepperController.setStepperOff();
    vTaskSuspend(stepperControllTask);
  }
  serialLogger.log(
      currentTime - lastTime,
      apps1.convertedValue(),
      apps2.convertedValue(),
      tps1.convertedValue(),
      tps2.convertedValue(),
      plausibilityValidator.isValid(),
      gErrorHandler.errorsToStr());
  lastTime = currentTime;
}