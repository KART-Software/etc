#include "toggle_switch.hpp"

ToggleSwitch::ToggleSwitch(uint8_t pin) : pin(pin) {}

void ToggleSwitch::initialize()
{
    pinMode(pin, INPUT_PULLUP);
    state = digitalRead(pin);
    lastState = state;
    _isOn = state == onState;
    _switched = false;
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

SelectSwitch3Pin::SelectSwitch3Pin(uint8_t pin1, uint8_t pin2, uint8_t pin3) : toggleSwitch1(pin1), toggleSwitch2(pin2), toggleSwitch3(pin3)
{
}

void SelectSwitch3Pin::initialize()
{
    toggleSwitch1.initialize();
    toggleSwitch2.initialize();
    toggleSwitch3.initialize();
}

void SelectSwitch3Pin::read()
{
    toggleSwitch1.read();
    toggleSwitch2.read();
    toggleSwitch3.read();
}

bool SelectSwitch3Pin::changed()
{
    return toggleSwitch1.switched() || toggleSwitch2.switched() || toggleSwitch3.switched();
}

SelectSwitch3Pin::Status SelectSwitch3Pin::getStatus()
{
    if (toggleSwitch1.isOn())
    {
        return SelectSwitch3Pin::Status::First;
    }
    else if (toggleSwitch2.isOn())
    {
        return SelectSwitch3Pin::Status::Second;
    }
    else if (toggleSwitch3.isOn())
    {
        return SelectSwitch3Pin::Status::Third;
    }
    else
    {
        return SelectSwitch3Pin::Status::Zero;
    }
}