#include <Arduino.h>
#include <mcp_can.h>
#include <SPI.h>

#define MOTION_SENSOR_PIN 1

void setup()
{
}

void loop()
{
  int motionSensorValue = analogRead(MOTION_SENSOR_PIN);
}