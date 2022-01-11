#pragma once

#include "dsp.hpp"

namespace Fib::Dsp
{

template <typename T> class RangedValue
{
public:
    constexpr RangedValue(T const &initialValue, T const &lowerLimit, T const &upperLimit,
                          std::function<bool(T const &value)> onChangeF = nullptr)
        : value(initialValue), lowerLimit(lowerLimit), upperLimit(upperLimit), onChangeF(onChangeF)
    {
    }
    bool isInRange(T value) const
    {
        return (this->lowerLimit <= value && value <= this->upperLimit);
    };

    T get()
    {
        return this->value;
    }

    bool set(T value)
    {
        bool result = false;
        if (this->isInRange(value))
        {
            this->value = value;
            result = onChangeF ? this->onChangeF(this->value) : true;
        }
        return result;
    }

    constexpr T getLowerLimit() const
    {
        return this->lowerLimit;
    }
    constexpr T getUpperLimit() const
    {
        return this->upperLimit;
    }

private:
    T value;
    const T lowerLimit, upperLimit;
    std::function<bool(T const &value)> onChangeF;
};
} // namespace Fib::Dsp