/*
 * Copyright (c) 2025 Lukas Neverauskis https://github.com/lukasnee
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include "resources.hpp"

#include <limits>
// NOTE: peripheral instances should get constructer after MCU core and freertos initialization

bool Periph::init() { return getUart2Stream().init(); }

// TODO: cleanup tim6 mess below, put this logic to lower level source file
// For FreeRTOS TimeGetStats
Tim6 &Periph::getTim6() {
    // Simple 16-bit value counted-up in 10 KHz
    static Tim6 tim6((SystemCoreClock / (10000)), (std::numeric_limits<std::uint16_t>::max()));
    return tim6;
}
extern "C" void vConfigureTimerForRunTimeStats() { Periph::getTim6().start(); }

extern "C" uint32_t uint32GetRunTimeCounterValue() {
    return static_cast<std::uint32_t>(Periph::getTim6().getCounterValue());
}

// For Shell and Logger
Uart2 &Periph::getUart2() {
    static Uart2 uart2(921600);
    return uart2;
}

IOStream &Periph::getUart2Stream() {
    static IOStream iostream_uart2(Periph::getUart2());
    return iostream_uart2;
}

Uart3 &Periph::getUart3() {
    static Uart3 uart3(921600);
    return uart3;
}

// For Audio Stream 1
I2s2 &Periph::getI2s2() { return I2s2::getInstance(); }

Adc2 &Periph::getAdc2() { return Adc2::getInstance(); }
