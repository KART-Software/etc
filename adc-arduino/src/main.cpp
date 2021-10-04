#include <Arduino.h>
#include <Wire.h>

#define SELF_ADDR 8
#define NUMBER_OF_BYTES_SENT 8
#define NUMBER_OF_VALUES_SENT 4
#define ANALOG(i) i + 14

void reply();

void setup()
{
  Wire.begin(SELF_ADDR); // join i2c bus with address #8
  Wire.onRequest(reply); // register event
  Serial.begin(9600);
}

void loop()
{
  for (int i = 0; i < NUMBER_OF_VALUES_SENT; i++)
  {
    Serial.print(analogRead(ANALOG(i)));
    Serial.print("  ");
  }
  Serial.println();
  delay(100);
}

uint16_t value[NUMBER_OF_VALUES_SENT];
byte sendData[NUMBER_OF_BYTES_SENT];

void reply()
{
  for (int i = 0; i < NUMBER_OF_VALUES_SENT; i++)
  {
    value[i] = analogRead(ANALOG(i));
  }

  for (int i = 0; i < NUMBER_OF_VALUES_SENT; i++)
  {
    sendData[2 * i] = highByte(value[i]);
    sendData[2 * i + 1] = lowByte(value[i]);
  }
  Wire.write(sendData, NUMBER_OF_BYTES_SENT);
}