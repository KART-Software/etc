#include <Arduino.h>

#include "error_handler.hpp"
#include "constants.hpp"
#include "sensors.hpp"
#include "plausibility_validator.hpp"
#include "motor_controller.hpp"
#include "toggle_switch.hpp"
#include "configurator.hpp"

TaskHandle_t motorControllTask;
TaskHandle_t serialLoggingTask;
TaskHandle_t calibrationTask;

SelectSwitch3Pin selectSwitch;
Apps apps1(APPS_1_RAW_MIN, APPS_1_RAW_MAX, APPS_1_CH);
Apps apps2(APPS_2_RAW_MIN, APPS_2_RAW_MAX, APPS_2_CH);
Tps tps1(TPS_1_RAW_MIN, TPS_1_RAW_MAX, TPS_1_CH);
Tps tps2(TPS_2_RAW_MIN, TPS_2_RAW_MAX, TPS_2_CH);
Ittr ittr = Ittr();
Bps bps = Bps();
Target target(apps1, ittr);

PlausibilityValidator plausibilityValidator(apps1, apps2, tps1, tps2, target, bps);
MotorController motorController(target, tps1);
Configurator configurator(apps1, apps2, tps1, tps2, ittr, target, motorController, plausibilityValidator);

void setup()
{
  pinMode(FUEL_PUMP_PIN, OUTPUT);
  digitalWrite(FUEL_PUMP_PIN, HIGH);
  gAdc.begin();
  selectSwitch.initialize();
  configurator.initialize();
  configurator.calibrateFromFlash();
  xTaskCreatePinnedToCore(startWatingCalibration, "CalibrationTask", 8192, (void *)&configurator, 3, &calibrationTask, 0);

  bool motorOnAllowed = true;
  switch (selectSwitch.getStatus())
  {
  case SelectSwitch3Pin::Status::Zero:
    target.setModeCalibration();
    break;
  case SelectSwitch3Pin::Status::First:
    target.setModeNormal();
    break;
  case SelectSwitch3Pin::Status::Second:
    target.setModeRestricted();
    break;
  case SelectSwitch3Pin::Status::Third:
    target.setModeCalibration();
    motorOnAllowed = false;
    break;
  default:
    break;
  }
  motorController.initialize();
  if (motorOnAllowed)
  {
    motorController.setMotorOn();
  }
  // delay(10);
  plausibilityValidator.initialize();
  xTaskCreatePinnedToCore(startLogging, "SerialLoggingTask", 8192, (void *)&plausibilityValidator, 2, &serialLoggingTask, 0);
  xTaskCreatePinnedToCore(startMotor, "MotorConstrollTask", 8192, (void *)&motorController, 1, &motorControllTask, 1);
  if (!motorOnAllowed)
  {
    vTaskSuspend(motorControllTask);
  }
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
  selectSwitch.read();

  if (!plausibilityValidator.isCurrentlyValid())
  {
    if (motorController.isOn())
    {
      motorController.setMotorOff();
      vTaskSuspend(motorControllTask);
      digitalWrite(FUEL_PUMP_PIN, LOW);
    }
  }
  if (selectSwitch.changed())
  {
    switch (selectSwitch.getStatus())
    {
    case SelectSwitch3Pin::Status::Zero:
      target.setModeCalibration();
      break;
    case SelectSwitch3Pin::Status::First:
      target.setModeNormal();
      break;
    case SelectSwitch3Pin::Status::Second:
      target.setModeRestricted();
      break;
    case SelectSwitch3Pin::Status::Third:
      target.setModeNormal();
      break;
    default:
      break;
    }
  }
}