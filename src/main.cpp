#include <Arduino.h>

#include "error_handler.hpp"
#include "constants.hpp"
#include "sensors.hpp"
#include "plausibility_validator.hpp"
#include "motor_controller.hpp"
#include "toggle_switch.hpp"
#include "configurator.hpp"
// #include "moving_average.hpp"

TaskHandle_t motorControllTask;
TaskHandle_t serialLoggingTask;
TaskHandle_t calibrationTask;

ToggleSwitch toggleSwitch(BUTTON_3_PIN);
ToggleSwitch toggleSwitch2(BUTTON_2_PIN);
Apps apps1(APPS_1_RAW_MIN, APPS_1_RAW_MAX, APPS_1_CH);
Apps apps2(APPS_2_RAW_MIN, APPS_2_RAW_MAX, APPS_2_CH);
Tps tps1(TPS_1_RAW_MIN, TPS_1_RAW_MAX, TPS_1_CH);
Tps tps2(TPS_2_RAW_MIN, TPS_2_RAW_MAX, TPS_2_CH);
Ittr ittr = Ittr();
Bps bps = Bps();
TargetSensor targetSensor(apps1, ittr);

PlausibilityValidator plausibilityValidator(apps1, apps2, tps1, tps2, targetSensor, bps);
MotorController motorController(targetSensor, tps1);
Configurator configurator(apps1, apps2, tps1, tps2, ittr, targetSensor, motorController, plausibilityValidator);

void setup()
{
  pinMode(FUEL_PUMP_PIN, OUTPUT);
  digitalWrite(FUEL_PUMP_PIN, HIGH);
  gAdc.begin();
  toggleSwitch.initialize();
  toggleSwitch2.initialize();
  configurator.initialize();
  plausibilityValidator.initialize();
  configurator.calibrateFromFlash();
  xTaskCreatePinnedToCore(startWatingCalibration, "CalibrationTask", 8192, (void *)&configurator, 3, &calibrationTask, 0);
  xTaskCreatePinnedToCore(startLogging, "SerialLoggingTask", 8192, (void *)&plausibilityValidator, 2, &serialLoggingTask, 0);
  apps1.setIdling(toggleSwitch.isOn());
  apps2.setIdling(toggleSwitch.isOn());
  ittr.setIdling(toggleSwitch.isOn());
  apps1.setRestricted(toggleSwitch2.isOn());
  apps2.setRestricted(toggleSwitch2.isOn());
  ittr.setRestricted(toggleSwitch2.isOn());
  motorController.initialize();
  motorController.setMotorOn();
  xTaskCreatePinnedToCore(startMotor, "MotorConstrollTask", 8192, (void *)&motorController, 1, &motorControllTask, 1);
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
  toggleSwitch.read();
  toggleSwitch2.read();

  if (!plausibilityValidator.isCurrentlyValid())
  {
    if (motorController.isOn())
    {
      motorController.setMotorOff();
      vTaskSuspend(motorControllTask);
      digitalWrite(FUEL_PUMP_PIN, LOW);
    }
  }
  if (toggleSwitch.switched())
  {
    apps1.setIdling(toggleSwitch.isOn());
    apps2.setIdling(toggleSwitch.isOn());
    ittr.setIdling(toggleSwitch.isOn());
  }
  if (toggleSwitch2.switched())
  {
    apps1.setRestricted(toggleSwitch2.isOn());
    apps2.setRestricted(toggleSwitch2.isOn());
    ittr.setRestricted(toggleSwitch2.isOn());
  }
}