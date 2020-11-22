/*
 Rotary encoder class for modular synth project Fibration.
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

#ifndef _ROTARY_ENCODER_H
#define _ROTARY_ENCODER_H

#include <cstdio>
#include "Hardware/gpio.hpp"

#define LATCHSTATE 3

class RotaryEncoder {
// public:
// 	// ----- Constructor -----
// 	RotaryEncoder(Pin pin1, Pin pin2);
// 	RotaryEncoder(Pin pin1, Pin pin2, Pin pinButton);
// 	// retrieve the current position
// 	uint32_t getPosition();

// 	// simple retrieve of the direction the knob was rotated at.
// 	// 0 = No rotation, 1 = Clockwise, -1 = Counter Clockwise
// 	int8_t getDirection();

// 	// adjust the current position
// 	void setPosition(uint32_t newPosition);

// 	// call this function every some milliseconds or
// 	// by using an interrupt for handling state changes of the rotary encoder.
// 	void tick();

// private:
// 	GPIO _pin1(), _pin2(), _pinBtn();
// 	volatile int8_t _oldState;
// 	volatile uint32_t _position;     // Internal position (4 times _positionExt)
// 	volatile uint32_t _positionExt;      // External position
// 	volatile uint32_t _positionExtPrev; // External position (used only for direction checking)

};

#endif // _ROTARY_ENCODER_H
