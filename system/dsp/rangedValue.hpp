#pragma once

#include "dsp.hpp"

namespace Fib::Dsp {

template <typename T> class RangedValue {
public:
    using OnChangeF = std::function<void(T const &value)>;

    constexpr RangedValue(T const &initialValue, T const &lowerLimit, T const &upperLimit, OnChangeF onChangeF = nullptr)
        : value(initialValue), m_lowerLimit(lowerLimit), m_upperLimit(upperLimit), onChangeF(onChangeF) {}

    bool isInRange(T value) const { return (this->m_lowerLimit <= value && value <= this->m_upperLimit); };

    T get() { return this->value; }

    void set(T value) {
        if (this->isInRange(value)) {
            this->value = value;
            if (this->onChangeF) {
                this->onChangeF(this->value);
            }
        }
    }

    constexpr T lowerLimit() const { return this->m_lowerLimit; }
    constexpr T upperLimit() const { return this->m_upperLimit; }

private:
    T value;
    const T m_lowerLimit, m_upperLimit;
    OnChangeF onChangeF;
};
} // namespace Fib::Dsp