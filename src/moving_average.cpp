#include "moving_average.hpp"

MovingAverage::MovingAverage(uint size) : size(size)
{
    value = new float[size];
}

void MovingAverage::add(float value)
{
    this->value[index] = value;
    if (index >= size - 1)
    {
        index = 0;
        sizeFull = true;
    }
    else
    {
        index++;
    }
}

float MovingAverage::getAvg()
{
    if (sizeFull)
    {
        return getAvg(size);
    }
    else
    {
        return getAvg(index);
    }
}

float MovingAverage::getAvg(uint size)
{
    float sum = 0;
    for (int i = 0; i < size; i++)
    {
        sum += value[i];
    }
    return sum / size;
}