#ifndef _SERVO_CONTROLLER_H_
#define _SERVO_CONTROLLER_H_

#include <ESP32Servo.h>
#include <M5Stack.h>

#include "init_pins.hpp"
#include "sensors.hpp"

#define SERVO_CYCLE_TIME_MS 100

#define SERVO_PULSE_WIDTH_MICROS_MIN 500
#define SERVO_PULSE_WIDTH_MICROS_MAX 2500

#define SERVO_PWM_FREQUENCY 200

#define SERVO_ANGLE_MIN 0
#define SERVO_ANGLE_MAX 180

class ServoController
{
public:
    enum State
    {
        Off,
        On
    };
    ServoController(Apps *apps1, Apps *apps2, Tps *tps1, Tps *tps2);
    void control();
    void setServoOn();
    void setServoOff();
    void start();
    enum State getState();

private:
    Servo servo;
    Apps *apps1, *apps2;
    Tps *tps1, *tps2;
    double intercept, slope;
    double tp, output, app;
    void setConversion();
    int convertToAngle(double app);
    enum State state;
};

void startServo(void *servoController);

#endif