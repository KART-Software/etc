#include <Arduino.h>
#include <mcp_can.h>
#include <SPI.h>

MCP_CAN CAN(10);

void setup()
{
  Serial.begin(115200);

START_INIT:

  if (CAN_OK == CAN.begin(CAN_500KBPS)) // init can bus : baudrate = 500k
  {
    Serial.println("CAN BUS Shield init ok!");
  }
  else
  {
    Serial.println("CAN BUS Shield init fail");
    Serial.println("Init CAN BUS Shield again");
    delay(100);
    goto START_INIT;
  }
}

void loop()
{
}