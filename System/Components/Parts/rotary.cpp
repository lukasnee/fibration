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

#include "Hardware/gpio.hpp"

#include "rotary.hpp"

// The array holds the values �1 for the entries where a position was decremented,
// a 1 for the entries where the position was incremented
// and 0 in all the other (no change or not valid) cases.

const int8_t KnobDirection[] = { 0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0 };

// positions: [3] 1 0 2 [3] 1 0 2 [3]
// [3] is the positions where my rotary switch detends
// ==> right, count up
// <== left,  count down

// RotaryEncoder::init()
// {
// 	// when not started in motion, the current state of the encoder should be 3
// 	_oldState = 3;

// 	_position = 0;
// 	_positionExt = 0;
// 	_positionExtPrev = 0;
// }

// RotaryEncoder::RotaryEncoder(Pin pin1, Pin pin2)
// {
// 	init();
// 	_pin1().init();
// }

// RotaryEncoder::RotaryEncoder(Pin pin1, Pin pin2, Pin pinButton)
// {
// 	init();
// }

// uint32_t RotaryEncoder::getPosition()
// {
// 	return _positionExt;
// }

// int8_t RotaryEncoder::getDirection()
// {
// 	int8_t ret = 0;
// 	if (_positionExtPrev > _positionExt)
// 	{
// 		ret = -1;
// 		_positionExtPrev = _positionExt;
// 	}
// 	else if (_positionExtPrev < _positionExt)
// 	{
// 		ret = 1;
// 		_positionExtPrev = _positionExt;
// 	} else
// 	{
// 		ret = 0;
// 		_positionExtPrev = _positionExt;
// 	}

// 	return ret;
// }

// void RotaryEncoder::setPosition(uint32_t newPosition)
// {
// 	// only adjust the external part of the position.
// 	_position = ((newPosition << 2) | (_position & 0x03L));
// 	_positionExt = newPosition;
// 	_positionExtPrev = newPosition;
// }

// void RotaryEncoder::tick()
// {
// 	uint8_t sig1 = _pin1.digitalRead();
// 	uint8_t sig2 = _pin2.digitalRead();

// 	int8_t thisState = sig1 | (sig2 << 1);

// 	if (_oldState != thisState) {
// 		_position += KnobDirection[thisState | (_oldState << 2)];

// 		if (thisState == LATCHSTATE)
// 			_positionExt = _position >> 2;

// 		_oldState = thisState;
// 	}
// }
