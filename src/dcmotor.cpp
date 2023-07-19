#include "dcmotor.hpp"

DcMotor::DcMotor(uint8_t pwmPin1, uint8_t pwmPin2, uint8_t ch1, uint8_t ch2) : pwmPin1(pwmPin1), pwmPin2(pwmPin2), ch1(ch1), ch2(ch2)
{
}

void DcMotor::initialize()
{
    ledcSetup(ch1, PWM_LEDC_FREQUENCY, PWM_LEDC_RESOLUTION);
    ledcSetup(ch2, PWM_LEDC_FREQUENCY, PWM_LEDC_RESOLUTION);
}

void DcMotor::setScaleMax(double scaleMax)
{
    this->scaleMax = abs(scaleMax);
}

void DcMotor::setOutputMax(uint16_t outputMax)
{
    this->outputMax = outputMax;
}

void DcMotor::write(double value)
{
    uint16_t duty1, duty2;
    if (value > 0)
    {
        duty1 = min(outputMax, outputMax * abs(value) / scaleMax);
        duty2 = 0;
    }
    else
    {
        duty1 = 0;
        duty2 = min(outputMax, outputMax * abs(value) / scaleMax);
    }
    write(duty1, duty2);
}

void DcMotor::write(uint16_t duty1, uint16_t duty2)
{
    ledcWrite(ch1, duty1);
    ledcWrite(ch2, duty2);
}

double DcMotor::min(double a, double b)
{
    return ((a) < (b) ? (a) : (b));
}

void DcMotor::on()
{
    ledcAttach();
    isOn = true;
}

void DcMotor::off()
{
    write(0, 0);
    ledcDetach();
    isOn = false;
}

void DcMotor::ledcAttach()
{
    ledcAttachPin(pwmPin1, ch1);
    ledcAttachPin(pwmPin2, ch2);
}

void DcMotor::ledcDetach()
{
    ledcDetachPin(pwmPin1);
    ledcDetachPin(pwmPin2);
}