#ifndef _TOGGLE_SWITCH_H_
#define _TOGGLE_SWITCH_H_
#include <Arduino.h>
#include <constants.hpp>

#define SWITCH_DURATION 100 // ms

class ToggleSwitch // プルアップピン（通常HIGHでGNDと繋がってLOWになる）を使う想定のスイッチのクラス
{
public:
    ToggleSwitch(uint8_t pin);
    void initialize();
    void read();
    bool isOn();
    bool switched();
    bool switchedToOff();
    bool switchedToOn();

private:
    const uint8_t pin;
    const uint32_t duration = SWITCH_DURATION;
    const uint8_t onState = LOW;
    bool _isOn;
    bool _switched;
    uint8_t state = !onState;     // current button state
    uint8_t lastState = !onState; // previous button state
    uint32_t time = 0;            // time of current state (all times are in ms)
    uint32_t lastTime = 0;        // time of previous state
    uint32_t lastChange = 0;      // time of last state change
};

class SelectSwitch3Pin
{
public:
    enum class Status
    {
        Zero,
        First,
        Second,
        Third
    };
    SelectSwitch3Pin(uint8_t pin1 = BUTTON_1_PIN, uint8_t pin2 = BUTTON_2_PIN, uint8_t pin3 = BUTTON_3_PIN);
    void initialize();
    void read();
    bool changed();
    Status getStatus();

private:
    ToggleSwitch toggleSwitch1, toggleSwitch2, toggleSwitch3;
};

#endif