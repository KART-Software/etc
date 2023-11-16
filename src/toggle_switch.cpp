#include "toggle_switch.hpp"

ToggleSwitch::ToggleSwitch(uint8_t pin) : pin(pin) {}

void ToggleSwitch::initialize()
{
    pinMode(pin, INPUT_PULLUP);
}

void ToggleSwitch::read()
{
    lastState = state;
    state = digitalRead(pin);
    lastTime = time;
    time = millis();
    if (state != lastState)
    {
        lastChange = time;
    }
    if ((time - lastChange > duration) && ((state == onState) != _isOn))
    {
        _isOn = (state == onState);
        _switched = true;
    }
    else
    {
        _switched = false;
    }
}

bool ToggleSwitch::isOn()
{
    return _isOn;
}

bool ToggleSwitch::switched()
{
    return _switched;
}

bool ToggleSwitch::switchedToOn()
{
    return _switched && _isOn;
}

bool ToggleSwitch::switchedToOff()
{
    return _switched && !_isOn;
}