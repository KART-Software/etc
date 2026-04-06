#pragma once
#include "core_pins.h"

template <int PIN, bool INVERTED = false>
class FastPin
{
public:
    static void init()
    {
        pinMode(PIN, OUTPUT);
        clear();
    }

    static void set()
    {
        digitalWriteFast(PIN, HIGH ^ INVERTED);
    }

    static void clear()
    {
        digitalWriteFast(PIN, LOW ^ INVERTED);
    }
};