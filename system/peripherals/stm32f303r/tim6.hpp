// Copyright (c) 2026 Lukas Neverauskis <lukas.neverauskis@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only

#pragma once

#include <cstdint>

class Tim6
{
public:
    typedef std::uint32_t CounterType;

    Tim6(std::uint16_t prescaler, std::uint16_t period);
    void start();
    void overflowCallback();
    CounterType getCounterValue() const;
    void stop();
    void operator=(Tim6 const&) = delete;
    Tim6(Tim6 const&) = delete;
    ~Tim6();
private:
    CounterType counterValue;
};
