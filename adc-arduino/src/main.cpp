#include <Arduino.h>
#include <Wire.h>

#define SELF_ADDR 8
#define NUMBER_OF_BYTES_SENT 8

void reply();

void setup()
{
  Wire.begin(SELF_ADDR); // join i2c bus with address #8
  Wire.onRequest(reply); // register event
  Serial.begin(9600);
}

void loop()
{
  delay(100);
}

uint16_t value[4];
byte sendData[8];

void reply()
{
  Serial.print("requested");
  value[0] = analogRead(A0);
  value[1] = analogRead(A1);
  value[2] = analogRead(A2);
  value[3] = analogRead(A4);

  for (int i = 0; i < 4; i++)
  {
    sendData[2 * i] = highByte(value[i]);
    sendData[2 * i + 1] = lowByte(value[0]);
  }
  Wire.write(sendData, 8);
}