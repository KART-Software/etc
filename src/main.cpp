#include <Arduino.h>

#include "error_handler.hpp"
#include "constants.hpp"
#include "sensors.hpp"
#include "plausibility_validator.hpp"
#include "serial_logger.hpp"
#include "motor_controller.hpp"
// #include "moving_average.hpp"

TaskHandle_t motorControllTask;
TaskHandle_t serialLoggingTask;

Apps apps1(APPS_1_RAW_MIN, APPS_1_RAW_MAX, APPS_1_CH);
Apps apps2(APPS_2_RAW_MIN, APPS_2_RAW_MAX, APPS_2_CH);
Tps tps1(TPS_1_RAW_MIN, TPS_1_RAW_MAX, TPS_1_CH);
Tps tps2(TPS_2_RAW_MIN, TPS_2_RAW_MAX, TPS_2_CH);
Ittr ittr = Ittr();
Bps bps = Bps();
#ifdef IST_CONTROLLER
PlausibilityValidator plausibilityValidator(apps1, apps2, tps1, tps2, ittr, bps);
MotorController motorController(ittr, tps1);
#else
PlausibilityValidator plausibilityValidator(apps1, apps2, tps1, tps2, bps);
MotorController motorController(apps1, tps1);
#endif

// MovingAverage movingAverage[4] = {MovingAverage(), MovingAverage(), MovingAverage(), MovingAverage()};

void setup()
{
  gAdc.begin();
  motorController.initialize();
  motorController.setMotorOn();
  // xTaskCreatePinnedToCore(startManualAdjusterMode, "SensorManualAdjustTask", 8192, (void *)&motorController, 1, &sensorManualAdjustTask, 1);
  // vTaskSuspend(sensorManualAdjustTask);
  xTaskCreatePinnedToCore(startMotor, "MotorConstrollTask", 8192, (void *)&motorController, 1, &motorControllTask, 0);
  plausibilityValidator.initialize();
  xTaskCreatePinnedToCore(startLogging, "SerialLoggingTask", 8192, (void *)&plausibilityValidator, 2, &serialLoggingTask, 1);
  // lastTime = micros();
}

void loop()
{
  gAdc.read();
  apps1.read();
  apps2.read();
  ittr.read();
  tps1.read();
  tps2.read();
  bps.read();

  if (!plausibilityValidator.isCurrentlyValid())
  {
    if (motorController.isOn())
    {
      motorController.setMotorOff();
      vTaskSuspend(motorControllTask);
    }
  }
}