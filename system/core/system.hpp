/*
 * Copyright (c) 2025 Lukas Neverauskis https://github.com/lukasnee
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include "ln/shell/CLI.hpp"

#include "FreeRTOS/Task.hpp"

extern "C"
{
#include "system.h"
}

#include <cstdint>

class FibSys : public FreeRTOS::Task {
public:
    // entry point of fibration system, should be called in main
    static void boot();

    static ln::shell::CLI &getCliInstance() {
        static std::array<char, 256> input_line_buf;
        static std::array<char, 256> history_buf;
        static ln::shell::CLI instance{input_line_buf, history_buf};
        return instance;
    }

    static void hardwareReboot();

    // 1KHz system tick time reference
    static void getUptime(std::uint32_t &days, std::uint32_t &hours, std::uint32_t &minutes, std::uint32_t &seconds,
                          std::uint32_t &milliseconds);

    enum Priority : BaseType_t {
        lowest = 0,
        veryLow = 1,
        low = 2,
        medium = 3,
        high = 4,
        veryHigh = 5,
        highest = 6,

        // configuration
        appLow = lowest,
        appMedium = veryLow,
        appHigh = low,
        sysLow = medium,
        sysMedium = high,
        sysHigh = veryHigh,
        audioStream = sysLow,

        _enumSize
    };

    static_assert(Priority::_enumSize <= configMAX_PRIORITIES, "invalid priority configration!");

    FibSys(const FibSys &) = delete;
    FibSys(FibSys &&) = delete;

private:
    // can only be instantiated with FibSys::boot()
    FibSys(std::uint16_t stackDepth, BaseType_t priority);

    void startup();

    // FibSys thread code
    virtual void taskFunction() override;
};
