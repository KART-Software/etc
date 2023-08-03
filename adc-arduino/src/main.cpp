#include <Arduino.h>
#include <Wire.h>

#define SELF_ADDR 8
#define NUMBER_OF_BYTES_SENT 10
#define NUMBER_OF_VALUES_SENT 5
#define ANALOG(i) i + 54
#define OUTPUT_PER_COUNT 1000

void reply();
uint16_t value[NUMBER_OF_VALUES_SENT];
byte sendData[NUMBER_OF_BYTES_SENT];
int count = 0;

void setup()
{
  Wire.begin(SELF_ADDR); // join i2c bus with address #8
  Wire.onRequest(reply); // register event
  Serial.begin(9600);
}

void loop()
{
  count++;
  for (int i = 0; i < NUMBER_OF_VALUES_SENT; i++)
  {
    value[i] = analogRead(ANALOG(i));
  }
  if (count > OUTPUT_PER_COUNT)
  {
    count = 0;
    String logStr = "";
    for (int i = 0; i < NUMBER_OF_VALUES_SENT; i++)
    {
      logStr = logStr + String(value[i]) + "  ";
    }
    Serial.println(logStr);
  }
}

void reply()
{
  for (int i = 0; i < NUMBER_OF_VALUES_SENT; i++)
  {
    sendData[2 * i] = highByte(value[i]);
    sendData[2 * i + 1] = lowByte(value[i]);
  }
  Wire.write(sendData, NUMBER_OF_BYTES_SENT);
}