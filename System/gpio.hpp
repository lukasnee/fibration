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

namespace Hardware
{

class GPIO
{
public:
	enum Pin
	{
		PIN_0,
		PIN_1,
		PIN_2,
		PIN_3,
		PIN_4,
		PIN_5,
		PIN_6,
		PIN_7,
		PIN_8,
		PIN_9,
		PIN_A,
		PIN_B,
		PIN_C,
//		PIN_D, // used for SWIO
//		PIN_E, // used for SWCLK
		PIN_F,

		PIN_10,
		PIN_11,
//		PIN_12, // used for BOOT1
		PIN_13,
		PIN_14,
		PIN_15,
		PIN_16,
		PIN_17,
		PIN_18,
		PIN_19,
		PIN_1A,
		PIN_1B,
		PIN_1C,
		PIN_1D,
		PIN_1E,
		PIN_1F,

		PIN_2C, PIN_LED = PIN_2C, // RobotDyn LED pin, active LOW 
		PIN_2D,
		PIN_2E,

	PIN_TOTAL};

	enum InitState
	{
		INIT_STATE_RESET,
		INIT_STATE_SET
	};

	enum PinMode
	{
		PIN_MODE_INPUT,
		PIN_MODE_OUTPUT
	};

	enum PinPull
	{
		PIN_PULL_NONE,
		PIN_PULL_UP,
		PIN_PULL_DOWN
	};

	GPIO(Pin pin, 
		InitState state = INIT_STATE_RESET,
		PinMode mode = PIN_MODE_OUTPUT,
		PinPull pull = PIN_PULL_NONE);
	~GPIO();
	void toggle();
	void digitalWrite(bool state);
	bool digitalRead();

private:		
	void init(
		InitState state = INIT_STATE_RESET,
		PinMode mode = PIN_MODE_OUTPUT,
		PinPull pull = PIN_PULL_NONE);
	void deinit();

	struct PinStatus
	{
		bool isInitialized;
	};

	Pin _pin;
	static PinStatus arPinStatus[PIN_TOTAL];
};
} //namespace Hardware
