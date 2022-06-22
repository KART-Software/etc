#include "servo_controller.hpp"

ServoController::ServoController(Apps *apps, Tps *tps)
    : servo(Servo()),
      apps(apps), tps(tps)
{
    servo.setPeriodHertz(SERVO_PWM_FREQUENCY);
    servo.attach(SERVO_PWM_PIN, SERVO_PULSE_WIDTH_MICROS_MIN, SERVO_PULSE_WIDTH_MICROS_MAX);
}

void ServoController::setConversion()
{
    double appsMax = apps->getMaxValue();
    double appsMin = apps->getMinValue();
    slope = (angleMax - angleMin) / (appsMax - appsMin);
    intercept = (appsMax * angleMin - appsMin * angleMax) / (appsMax - appsMin);
}

int ServoController::convertToAngle(double app)
{
    return slope * app + intercept;
}

void ServoController::control()
{
    double targetTp = apps->convertToTargetTp();
    angle = convertToAngle(targetTp);
    servo.write(angle);
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

int ServoController::getAngle()
{
    return angle;
}

void ServoController::rotate(int angle)
{
    this->angle = constrain(this->angle + angle, SERVO_ANGLE_LIMIT_MIN, SERVO_ANGLE_LIMIT_MAX);
    servo.write(this->angle);
}

void ServoController::initializeAngleRangeAutomatic()
{
    angle = INITIAL_SERVO_ANGLE;
    servo.write(angle);
    delay(300);
    // gAdc.read();
    tps->read();

    if (tps->convertedValue() < tps->getMinValue() || tps->getMaxValue() < tps->convertedValue())
    {
    }

    while (true)
    {
        // gAdc.read();
        tps->read();
        if (tps->convertedValue() < tps->getMinValue())
        {
            angleMin = angle + SERVO_DIRECTION * (1 + INITIALIZING_AMEND_ANGLE);
            break;
        }
        rotate(SERVO_DIRECTION * (-1));
        delay(20);
    }

    while (true)
    {
        // gAdc.read();
        tps->read();
        if (tps->getMaxValue() < tps->convertedValue())
        {
            angleMax = angle - SERVO_DIRECTION * (1 + INITIALIZING_AMEND_ANGLE);
            break;
        }
        rotate(SERVO_DIRECTION * 1);
        delay(20);
    }

    setConversion();
}

void startServo(void *servoController)
{
    ServoController *controller;
    controller = (ServoController *)servoController;
    controller->initializeAngleRangeAutomatic();
    controller->start();
}