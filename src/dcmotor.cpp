#include "dcmotor.hpp"

DcMotor::DcMotor() {}

#ifdef HILITAND

void DcMotor::initialize()
{
    ledcSetup(ch1, PWM_LEDC_FREQUENCY, PWM_LEDC_RESOLUTION);
    ledcSetup(ch2, PWM_LEDC_FREQUENCY, PWM_LEDC_RESOLUTION);
    pinMode(enablePin, OUTPUT);
    digitalWrite(enablePin, LOW);
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

void DcMotor::on()
{
    ledcAttach();
    digitalWrite(enablePin, HIGH);
    _isOn = true;
}

void DcMotor::off()
{
    write(0, 0);
    digitalWrite(enablePin, LOW);
    ledcDetach();
    _isOn = false;
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

#endif

#ifdef VNH5019
void DcMotor::initialize()
{
    ledcSetup(ch, PWM_LEDC_FREQUENCY, PWM_LEDC_RESOLUTION);
    pinMode(inAPin, OUTPUT);
    pinMode(inBPin, OUTPUT);
    digitalWrite(inAPin, LOW);
    digitalWrite(inBPin, LOW);
}

void DcMotor::write(double value)
{
    if (value > 0)
    {
        write(min(outputMax, outputMax * abs(value) / scaleMax), HIGH, LOW);
    }
    else
    {
        write(min(outputMax, outputMax * abs(value) / scaleMax), LOW, HIGH);
    }
}

void DcMotor::write(uint16_t duty, uint8_t inAState, uint8_t inBState)
{
    ledcWrite(ch, duty);
    digitalWrite(inAPin, inAState);
    digitalWrite(inBPin, inBState);
}

void DcMotor::on()
{
    ledcAttach();
    _isOn = true;
}

void DcMotor::off()
{
    write(0, LOW, LOW);
    ledcDetach();
    _isOn = false;
}

void DcMotor::ledcAttach()
{
    ledcAttachPin(pwmPin, ch);
}

void DcMotor::ledcDetach()
{
    ledcDetachPin(pwmPin);
}
#endif

bool DcMotor::isOn()
{
    return _isOn;
}

double DcMotor::min(double a, double b)
{
    return ((a) < (b) ? (a) : (b));
}