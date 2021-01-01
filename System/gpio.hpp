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
    enum Port : std::size_t
    {
        A, B, C, D, E, F,
    _PORT_ENUM_MAX = F
    };

    enum Pin : std::size_t
    {
        pin0, pin1, pin2, pin3, 
        pin4, pin5, pin6, pin7, 
        pin8, pin9, pin10, pin11, 
        pin12, pin13, pin14, pin15,

    _PIN_ENUM_MAX = pin15
    };

    enum Mode : std::size_t
    {
        input,
        outputPushPull,
        outputOpenDrain,
        // support more
    _MODE_ENUM_MAX = outputOpenDrain
    };

    enum Pull : std::size_t
    {
        none, up, down, 
    _PULL_ENUM_MAX = down
    };

    enum Speed : std::size_t
    {
        low, medium, high, 
    _SPEED_ENUM_MAX = high
    };

    enum class LogicState : std::size_t
    {
        low, high,
    _LOGICSTATE_ENUM_MAX = high 
    };

    Gpio(Port port, Pin pin);
    void init(Mode mode, Pull pull = Pull::none, Speed speed = Speed::low);
    void init(Mode mode, LogicState initState, Pull pull = Pull::none, Speed speed = Speed::low);
    bool read();
    void write(LogicState value);
    void toggle();
    void deinit();
    ~Gpio();

private:
    Port port;
    Pin pin;
    bool isInitialized;
};
