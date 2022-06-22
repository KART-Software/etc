#include <M5Stack.h>
// #include <mcp_can.h>
// #include <SPI.h>

#include "error_handler.hpp"
#include "init_pins.hpp"
#include "sensors.hpp"
#include "plausibility_validator.hpp"
#include "servo_controller.hpp"
#include "serial_logger.hpp"
#include "mode.hpp"
#include "manual_adjuster_mode.hpp"

TaskHandle_t servoControllTask;
TaskHandle_t sensorManualAdjustTask;

Mode mode;
Apps apps1(APPS_1_MIN, APPS_1_MAX, APPS_MARGIN, APPS_1_RAW_MIN, APPS_1_RAW_MAX, APPS_1_PIN);
Apps apps2(APPS_2_MIN, APPS_2_MAX, APPS_MARGIN, APPS_2_RAW_MIN, APPS_2_RAW_MAX, APPS_2_PIN);
Ittr ittr(ITTR_MIN, ITTR_MAX, ITTR_MARGIN, ITTR_RAW_MIN, ITTR_RAW_MAX, ITTR_PIN);
Tps tps1(TPS_1_MIN, TPS_1_MAX, TPS_MARGIN, TPS_1_RAW_MIN, TPS_1_RAW_MAX, TPS_1_PIN);
Tps tps2(TPS_2_MIN, TPS_2_MAX, TPS_MARGIN, TPS_2_RAW_MIN, TPS_2_RAW_MAX, TPS_2_PIN);
PlausibilityValidator plausibilityValidator(&apps1, &apps2, &tps1, &tps2);
ServoController servoController(&ittr, &tps1);
SerialLogger serialLogger;
unsigned long currentTime = 0;
unsigned long lastTime = 0;

void setup()
{
  mode = Mode::Regular;
  delay(500);
  M5.begin();
  serialLogger.initialize();
  initPins();
  gAdc.begin();
  servoController.setServoOn();
  xTaskCreatePinnedToCore(startManualAdjusterMode, "SensorManualAdjustTask", 8192, (void *)&servoController, 1, &sensorManualAdjustTask, 1);
  vTaskSuspend(sensorManualAdjustTask);
  xTaskCreatePinnedToCore(startServo, "ServoConstrollTask", 8192, (void *)&servoController, 1, &servoControllTask, 1);
}

void loop()
{
  currentTime = millis();
  gAdc.read();
  apps1.read();
  apps2.read();
  ittr.read();
  tps1.read();
  tps2.read();

  if (mode == Mode::Regular)
  {
    if (!plausibilityValidator.isCurrentlyValid())
    {
      servoController.setServoOff();
      vTaskSuspend(servoControllTask);
    }
    serialLogger.log(
        currentTime - lastTime,
        apps1.convertedValue(),
        apps2.convertedValue(),
        ittr.convertedValue(),
        tps1.convertedValue(),
        tps2.convertedValue(),
        plausibilityValidator.isValid(),
        gErrorHandler.errorsToStr());

    ENTER_ADJUSTER_MODE_BUTTON.read();
    if (ENTER_ADJUSTER_MODE_BUTTON.pressedFor(BUTTON_LONG_PRESS_THRESHOLD))
    {
      vTaskSuspend(servoControllTask);
      mode = Mode::Adjust;
      servoController.setServoOn();
      vTaskResume(sensorManualAdjustTask);
    }
  }
  else
  {
    serialLogger.log(
        apps1.getRawValue(),
        apps2.getRawValue(),
        ittr.getRawValue(),
        tps1.getRawValue(),
        tps2.getRawValue());
    ADJUSTER_MODE_FINISH_BUTTON.read();
    if (ADJUSTER_MODE_FINISH_BUTTON.pressedFor(BUTTON_LONG_PRESS_THRESHOLD))
    {
      vTaskSuspend(sensorManualAdjustTask);
      mode = Mode::Regular;
      vTaskResume(servoControllTask);
    }
    delay(30);
  }
  lastTime = currentTime;
}