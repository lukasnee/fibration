/*
    Fibration - modular synth framework
    Copyright (C) 2020 Lukas Neverauskis

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#pragma once

#define GPIO_CAREFUL true

#include "gpioIF.hpp"

#include <cstdint>
#include <functional>

enum class Pin
{
    A0,
    A1,
    A2,
    A3,
    A4,
    A5,
    A6,
    A7,
    A8,
    A9,
    A10,
    A11,
    A12,
    A13,
    A14,
    A15,

    B0,
    B1,
    B2,
    B3,
    B4,
    B5,
    B6,
    B7,
    B8,
    B9,
    B10,
    B11,
    B12,
    B13,
    B14,
    B15,

    C0,
    C1,
    C2,
    C3,
    C4,
    C5,
    C6,
    C7,
    C8,
    C9,
    C10,
    C11,
    C12,
    C13,
    C14, /* OSC32_IN */
    C15, /* OSC32_OUT */

    D0, /* N/A */
    D1, /* N/A */
    D2,
    D3,  /* N/A */
    D4,  /* N/A */
    D5,  /* N/A */
    D6,  /* N/A */
    D7,  /* N/A */
    D8,  /* N/A */
    D9,  /* N/A */
    D10, /* N/A */
    D11, /* N/A */
    D12, /* N/A */
    D13, /* N/A */
    D14, /* N/A */
    D15, /* N/A */
};

class Gpio : public GpioIF<Pin>
{

public:
    static Gpio &getInstance();

    void init(Pin pin, Mode mode, PinState pinStateInitial, Pull pull) override;
    [[nodiscard]] virtual inline bool read(Pin pin) override;
    inline void write(Pin pin, PinState pinState) override;
    inline void toggle(Pin pin);
    void deinit(Pin pin) override;

    /** @brief lock pin hardware configuration registers until mcu reboot */
    void lock(Pin pin);

protected:
    Gpio() = default;
    virtual ~Gpio() = default;
};
