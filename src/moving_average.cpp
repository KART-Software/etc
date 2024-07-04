#include "moving_average.hpp"

MovingAverage::MovingAverage(uint size) : size(size)
{
    value = new float[size];
    for (int i = 0; i < size; i++)
    {
        value[i] = 0;
    }
}

void MovingAverage::add(float value)
{
    this->value[index] = value;
    if (index >= size - 1)
    {
        index = 0;
        // sizeFull = true;
    }
    else
    {
        index++;
    }
}

float MovingAverage::getAvg()
{
    float sum = 0;
    for (int i = 0; i < size; i++)
    {
        sum += value[i];
    }
    return sum / size;
}