// Copyright (c) 2026 Lukas Neverauskis <lukas.neverauskis@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only

#include "resources.hpp"

// For Shell and Logger
Uart2 &Periph::getUart2() {
    static Uart2 uart2(921600);
    return uart2;
}

Uart3 &Periph::getUart3() {
    static Uart3 uart3(921600);
    return uart3;
}

// For Audio Stream 1
I2s2 &Periph::getI2s2() { return I2s2::getInstance(); }

Adc2 &Periph::getAdc2() { return Adc2::getInstance(); }
