#include "dcmotor.hpp"

DcMotor::DcMotor() {}

#ifdef HILITAND

void DcMotor::initialize()
{
    pinMode(enablePin, OUTPUT);
    digitalWrite(enablePin, LOW);
    pwmSetUp();
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
    if (!_isOn)
        return;
    analogWrite(pwmPin1, duty1);
    analogWrite(pwmPin2, duty2);
}

void DcMotor::on()
{
    digitalWrite(enablePin, HIGH);
    _isOn = true;
}

void DcMotor::off()
{
    write(0, 0);
    digitalWrite(enablePin, LOW);
    _isOn = false;
}

void DcMotor::pwmSetUp()
{
    pinMode(pwmPin1, OUTPUT);
    digitalWrite(pwmPin1, LOW);
    pinMode(pwmPin2, OUTPUT);
    digitalWrite(pwmPin2, LOW);
    analogWriteFrequency(pwmPin1, PWM_FREQUENCY);
    analogWriteFrequency(pwmPin2, PWM_FREQUENCY);
    analogWriteResolution(PWM_RESOLUTION);
}

#endif

#ifdef VNH5019
void DcMotor::initialize()
{
    pinMode(inAPin, OUTPUT);
    pinMode(inBPin, OUTPUT);
    digitalWrite(inAPin, LOW);
    digitalWrite(inBPin, LOW);
    pwmSetUp();
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
    if (!_isOn)
        return;
    analogWrite(pwmPin, duty);
    digitalWrite(inAPin, inAState);
    digitalWrite(inBPin, inBState);
}

void DcMotor::on()
{
    _isOn = true;
}

void DcMotor::off()
{
    write(0, LOW, LOW);
    _isOn = false;
}

void DcMotor::pwmSetUp()
{
    pinMode(pwmPin, OUTPUT);
    digitalWrite(pwmPin, LOW);
    analogWriteFrequency(pwmPin, PWM_FREQUENCY);
    analogWriteResolution(PWM_RESOLUTION);
}

#endif

#ifdef G2_18V17
void DcMotor::initialize()
{
    pinMode(relayPin, OUTPUT);
    digitalWrite(relayPin, LOW);
    pinMode(dirPin, OUTPUT);
    pinMode(slpPin, OUTPUT);
    digitalWrite(slpPin, LOW);
    pwmSetUp();
}

void DcMotor::write(double value)
{
    write(min(outputMax, outputMax * abs(value) / scaleMax), value < 0);
}

void DcMotor::write(uint16_t duty, uint8_t forward)
{
    if (!_isOn)
        return;
    analogWrite(pwmPin, duty);
    digitalWrite(dirPin, forward);
}

void DcMotor::on()
{
    digitalWrite(relayPin, HIGH);
    digitalWrite(slpPin, HIGH);
    _isOn = true;
}

void DcMotor::off()
{
    write(0, LOW);
    digitalWrite(relayPin, LOW);
    digitalWrite(slpPin, LOW);
    _isOn = false;
}

void DcMotor::pwmSetUp()
{
    pinMode(pwmPin, OUTPUT);
    digitalWrite(pwmPin, LOW);
    analogWriteFrequency(pwmPin, PWM_FREQUENCY);
    analogWriteResolution(PWM_RESOLUTION);
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

#ifdef DRV8256P

void DcMotor::initialize()
{
    pinMode(relayPin, OUTPUT);
    digitalWrite(relayPin, LOW);
    pinMode(slpPin, OUTPUT);
    digitalWrite(slpPin, LOW);
    pwmSetUp();
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
    if (!_isOn)
        return;
    analogWrite(pwmPin1, duty1);
    analogWrite(pwmPin2, duty2);
}

void DcMotor::on()
{
    digitalWrite(relayPin, HIGH);
    digitalWrite(slpPin, HIGH);
    _isOn = true;
}

void DcMotor::off()
{
    write(0, 0);
    digitalWrite(relayPin, LOW);
    digitalWrite(slpPin, LOW);
    _isOn = false;
}

void DcMotor::pwmSetUp()
{
    pinMode(pwmPin1, OUTPUT);
    digitalWrite(pwmPin1, LOW);
    pinMode(pwmPin2, OUTPUT);
    digitalWrite(pwmPin2, LOW);
    analogWriteFrequency(pwmPin1, PWM_FREQUENCY);
    analogWriteFrequency(pwmPin2, PWM_FREQUENCY);
    analogWriteResolution(PWM_RESOLUTION);
}

#endif

#ifdef TB67H450
void DcMotor::initialize()
{
    pinMode(relayPin, OUTPUT);
    digitalWrite(relayPin, LOW);
    pwmSetUp();
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
    if (!_isOn)
        return;
    analogWrite(pwmPin1, duty1);
    analogWrite(pwmPin2, duty2);
}

void DcMotor::on()
{
    digitalWrite(relayPin, HIGH);
    _isOn = true;
}

void DcMotor::off()
{
    write(0, 0);
    digitalWrite(relayPin, LOW);
    _isOn = false;
}

void DcMotor::pwmSetUp()
{
    pinMode(pwmPin1, OUTPUT);
    digitalWrite(pwmPin1, LOW);
    pinMode(pwmPin2, OUTPUT);
    digitalWrite(pwmPin2, LOW);
    analogWriteFrequency(pwmPin1, PWM_FREQUENCY);
    analogWriteFrequency(pwmPin2, PWM_FREQUENCY);
    analogWriteResolution(PWM_RESOLUTION);
}

#endif