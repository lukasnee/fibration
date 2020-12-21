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
		P0, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P13, P14, P15,

	_PIN_ENUM_MAX = P15
	};

	enum Mode : std::size_t
	{
		Input,
		Output_PP, // push/pull
		Output_OD, // open drain
		// support more
	_MODE_ENUM_MAX = Output_OD
	};

	enum Pull : std::size_t 
	{ 
		None, Up, Down, 
	_PULL_ENUM_MAX = Down
	};

	enum Speed : std::size_t 
	{ 
		Low, Medium, High, 
	_SPEED_ENUM_MAX = High 
	};

	Gpio(Port port, Pin pin);
	void init(bool initState = false, Mode mode = Output_PP, Pull pull = None, Speed speed = Low);
	bool read();
	void write(bool state);
	void toggle();
	void deinit();
	~Gpio();

private:
	Port port;
	Pin pin;
	Mode mode;
	Pull pull;
	Speed speed;
	bool isInitialized;
};
