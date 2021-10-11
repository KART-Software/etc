#include "servo_controller.hpp"

ServoController::ServoController(Apps *apps1, Apps *apps2, Tps *tps1, Tps *tps2)
    : servo(Servo()),
      apps1(apps1), apps2(apps2), tps1(tps1), tps2(tps2)
{
    servo.setPeriodHertz(SERVO_PWM_FREQUENCY);
    servo.attach(SERVO_PWM_PIN, SERVO_PULSE_WIDTH_MICROS_MIN, SERVO_PULSE_WIDTH_MICROS_MAX);
}

void ServoController::setConversion()
{
    double apps1Max = apps1->getMaxValue();
    double apps1Min = apps1->getMinValue();
    slope = (angleMax - angleMin) / (apps1Max - apps1Min);
    intercept = (apps1Max * angleMin - apps1Min * angleMax) / (apps1Max - apps1Min);
}

int ServoController::convertToAngle(double app)
{
    return slope * app + intercept;
}

void ServoController::control()
{
    app = apps1->convertedValue();
    double targetTp = appToTargetTp(app);
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
    gAdc.read();
    tps1->read();

    if (tps1->convertedValue() < tps1->getMinValue() || tps1->getMaxValue() < tps1->convertedValue())
    {
    }

    while (true)
    {
        gAdc.read();
        tps1->read();
        if (tps1->convertedValue() < tps1->getMinValue())
        {
            angleMin = angle + SERVO_DIRECTION * (1 + INITIALIZING_AMEND_ANGLE);
            break;
        }
        rotate(SERVO_DIRECTION * (-1));
        delay(20);
    }

    while (true)
    {
        gAdc.read();
        tps1->read();
        if (tps1->getMaxValue() < tps1->convertedValue())
        {
            angleMax = angle - SERVO_DIRECTION * (1 + INITIALIZING_AMEND_ANGLE);
            break;
        }
        rotate(SERVO_DIRECTION * 1);
        delay(20);
    }

    setConversion();
}

double ServoController::appToTargetTp(double app)
{
    return app;
}

void startServo(void *servoController)
{
    ServoController *controller;
    controller = (ServoController *)servoController;
    controller->start();
}