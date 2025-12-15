/*
 * Copyright (c) 2025 Lukas Neverauskis https://github.com/lukasnee
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include "ioStream.hpp"

#include "adc2.hpp"
#include "i2s2.hpp"
#include "tim6.hpp"
#include "uart2.hpp"
#include "uart3.hpp"

class Periph {
public:
    static bool init();

    static Tim6 &getTim6();
    static Uart2 &getUart2();
    static IOStream &getUart2Stream();
    static Uart3 &getUart3();
    static I2s2 &getI2s2();
    static Adc2 &getAdc2();
};
