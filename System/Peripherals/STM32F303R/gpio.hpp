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

#include <cstdint>

class Gpio
{
public:
    enum class Port : std::size_t
    {
        A, B, C, D, E, F,
    _PORT_ENUM_MAX = F
    };

    enum class Pin : std::size_t
    {
        pin0, pin1, pin2, pin3, 
        pin4, pin5, pin6, pin7, 
        pin8, pin9, pin10, pin11, 
        pin12, pin13, pin14, pin15,

    _PIN_ENUM_MAX = pin15
    };

    enum class Mode : std::size_t
    {
        input,
        outputPushPull,
        outputOpenDrain,
        // support more
    _MODE_ENUM_MAX = outputOpenDrain
    };

    enum class Pull : std::size_t
    {
        none, up, down, 
    _PULL_ENUM_MAX = down
    };

    enum class Speed : std::size_t
    {
        low, medium, high, 
    _SPEED_ENUM_MAX = high
    };

    enum class PinState : std::size_t
    {
        low, high, 
    _PINSTATE_ENUM_MAX = high 
    };

    Gpio(Port port, Pin pin);
    void init(Mode mode, Pull pull = Pull::none, Speed speed = Speed::low);
    void init(Mode mode, PinState initial, Pull pull = Pull::none, Speed speed = Speed::low);
    [[nodiscard]] bool read();
    void write(PinState value);
    void toggle();
    void lock(); // lock pin hardware configuration registers until mcu reboot 
    void deinit();
    ~Gpio();

private:
    Port port;
    Pin pin;
    bool isInitialized;
};
