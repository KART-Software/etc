#include "servo_controller.hpp"

ServoController::ServoController(Apps *apps1, Apps *apps2, Tps *tps1, Tps *tps2)
    : servo(Servo()),
      apps1(apps1), apps2(apps2), tps1(tps1), tps2(tps2)
{
    servo.setPeriodHertz(SERVO_PWM_FREQUENCY);
    servo.attach(SERVO_PWM_PIN, SERVO_PULSE_WIDTH_MICROS_MIN, SERVO_PULSE_WIDTH_MICROS_MAX);
    initializeAngleRangeAutomatic();
}

void ServoController::setConversion()
{
    double apps1Max = apps1->getMaxValue();
    double apps1Min = apps1->getMinValue();
    slope = (SERVO_ANGLE_MAX - SERVO_ANGLE_MIN) / (apps1Max - apps1Min);
    intercept = (apps1Max * SERVO_ANGLE_MIN - apps1Min * SERVO_ANGLE_MAX) / (apps1Max - apps1Min);
}

void ServoController::setConversion(int angleMin, int angleMax)
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
    angle = convertToAngle(app);
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
    this->angle = this->angle + angle;
    servo.write(this->angle);
}

void ServoController::initializeAngleRangeAutomatic()
{
    angle = INITIAL_SERVO_ANGLE;
    servo.write(angle);
    delay(300);
    gAdc.read();
    tps1->read();

    int angleMin, angleMax;

    if (tps1->convertedValue() < tps1->getMinValue() || tps1->getMaxValue() < tps1->convertedValue())
    {
    }

    while (true)
    {
        gAdc.read();
        tps1->read();
        if (tps1->convertedValue() < tps1->getMinValue())
        {
            angleMin = angle;
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
            angleMax = angle;
            break;
        }
        rotate(SERVO_DIRECTION * 1);
        delay(20);
    }

    setConversion(angleMin, angleMax);
}

void startServo(void *servoController)
{
    ServoController *controller;
    controller = (ServoController *)servoController;
    controller->start();
}