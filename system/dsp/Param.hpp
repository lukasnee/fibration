// Copyright (c)  2026 Lukas Neverauskis <lukas.neverauskis@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only

#pragma once

#include "dsp.hpp"

namespace Fib::Dsp::Param {

template <typename T> class Arithmetic {
public:
    static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type");

    constexpr Arithmetic(T const &defaultValue, T const &min, T const &max)
        : value(defaultValue), min{min}, max{max} {}

    T get() { return this->value; }

    void set(T value) {
        if (!this->isInRange(value)) {
            return;
        }
        this->value = value;
    }

    constexpr T get_min() const { return this->min; }
    constexpr T get_max() const { return this->max; }

private:
    bool isInRange(T value) const {
        return (this->min <= value && value <= this->max);
    };

    T value;
    const T min, max;
};
} // namespace Fib::Dsp::Param
