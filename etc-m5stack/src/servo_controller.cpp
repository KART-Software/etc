#include "servo_controller.hpp"

ServoController::ServoController(Apps *apps1, Apps *apps2, Tps *tps1, Tps *tps2)
    : servo(Servo()),
      apps1(apps1), apps2(apps2), tps1(tps1), tps2(tps2)
{
    servo.setPeriodHertz(SERVO_PWM_FREQUENCY);
    servo.attach(SERVO_PWM_PIN, SERVO_PULSE_WIDTH_MICROS_MIN, SERVO_PULSE_WIDTH_MICROS_MAX);
    setConversion();
}

void ServoController::setConversion()
{
    double apps1Max = apps1->getMaxValue();
    double apps1Min = apps1->getMinValue();
    slope = (SERVO_ANGLE_MAX - SERVO_ANGLE_MIN) / (apps1Max - apps1Min);
    intercept = (apps1Max * SERVO_ANGLE_MIN - apps1Min * SERVO_ANGLE_MAX) / (apps1Max - apps1Min);
}

int ServoController::convertToAngle(double app)
{
    return slope * app + intercept;
}

void ServoController::control()
{
    app = apps1->convertedValue();
    servo.write(convertToAngle(app));
}

void ServoController::setServoOn()
{
    digitalWrite(SERVO_POWER_PIN, HIGH);
    state = State::On;
}

void ServoController::setServoOff()
{
    digitalWrite(SERVO_POWER_PIN, LOW);
    state = State::Off;
}

void ServoController::start()
{
    while (1)
    {
        control();
        delay(SERVO_CYCLE_TIME_MS);
    }
}

enum ServoController::State ServoController::getState()
{
    return state;
}

void startServo(void *servoController)
{
    ServoController *controller;
    controller = (ServoController *)servoController;
    controller->start();
}