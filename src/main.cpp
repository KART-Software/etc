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

Apps apps1(APPS_1_MIN, APPS_1_MAX, APPS_MARGIN, APPS_1_RAW_MIN, APPS_1_RAW_MAX, APPS_1_CH);
Apps apps2(APPS_2_MIN, APPS_2_MAX, APPS_MARGIN, APPS_2_RAW_MIN, APPS_2_RAW_MAX, APPS_2_CH);
Tps tps1(TPS_1_MIN, TPS_1_MAX, TPS_MARGIN, TPS_1_RAW_MIN, TPS_1_RAW_MAX, TPS_1_CH);
Tps tps2(TPS_2_MIN, TPS_2_MAX, TPS_MARGIN, TPS_2_RAW_MIN, TPS_2_RAW_MAX, TPS_2_CH);
#ifdef IST_CONTROLLER
Ittr ittr(ITTR_MIN, ITTR_MAX, ITTR_MARGIN, ITTR_RAW_MIN, ITTR_RAW_MAX, ITTR_CH);
PlausibilityValidator plausibilityValidator(apps1, apps2, tps1, tps2, ittr);
MotorController motorController(ittr, tps1);
#else
PlausibilityValidator plausibilityValidator(apps1, apps2, tps1, tps2);
MotorController motorController(apps1, tps1);
#endif

// SerialLogger serialLogger;

// MovingAverage movingAverage[4] = {MovingAverage(), MovingAverage(), MovingAverage(), MovingAverage()};

void setup()
{
  // serialLogger.initialize();
  gAdc.begin();
  motorController.initialize();
  motorController.setMotorOn();
  // xTaskCreatePinnedToCore(startManualAdjusterMode, "SensorManualAdjustTask", 8192, (void *)&motorController, 1, &sensorManualAdjustTask, 1);
  // vTaskSuspend(sensorManualAdjustTask);
  xTaskCreatePinnedToCore(startMotor, "MotorConstrollTask", 8192, (void *)&motorController, 1, &motorControllTask, 0);
  xTaskCreatePinnedToCore(startLogging, "SerialLoggingTask", 8192, (void *)&plausibilityValidator, 2, &serialLoggingTask, 1);
  // lastTime = micros();
}

void loop()
{
  gAdc.read();
  apps1.read();
  apps2.read();
  // ittr.read();
  tps1.read();
  tps2.read();

  // if (!plausibilityValidator.isCurrentlyValid())
  // {
  //   motorController.setMotorOff();
  //   vTaskSuspend(motorControllTask);
  // }
}