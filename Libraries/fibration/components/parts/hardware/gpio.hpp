/*
    Digital I/O class for modular synth project Fibration.
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

#ifndef FIB_NCO_IO_IO_HPP_
#define FIB_NCO_IO_IO_HPP_

#include "stdint.h"

typedef enum pin_
{
	PIN_A,		PIN_B, 		PIN_C, 		PIN_D,
	PIN_E, 		PIN_F, 		PIN_G, 		PIN_H,
	PIN_I, 		PIN_J, 		PIN_K, 		PIN_L,
	PIN_M, 		PIN_N, 		PIN_O, 		PIN_P,
	PIN_Q, 		PIN_R, 		PIN_S, 		PIN_T,
	PIN_U, 		PIN_V, 		PIN_W, 		PIN_X,
	PIN_Y, 		PIN_Z, 		PIN_AA, 	PIN_AB,
	PIN_AC, 	PIN_AD, 	PIN_AE, 	PIN_AF,
	PIN_AG, 	PIN_AH, 	PIN_AI, 	PIN_AJ,
	PIN_AK, 	PIN_AL, 	PIN_AM, 	PIN_AN,
	PIN_AO, 	PIN_AP, 	PIN_AQ, 	PIN_AR,
	PIN_AS, 	PIN_AT, 	PIN_AU, 	PIN_AV,
	PIN_AW, 	PIN_AX, 	PIN_AY, 	PIN_AZ,
PIN_TOTAL} pin_e;

typedef enum pinMode_
{
	PIN_MODE_INPUT, PIN_MODE_OUTPUT
} pinMode_e;

typedef enum pinPull_
{
	PIN_PULL_NONE, PIN_PULL_UP, PIN_PULL_DOWN
} pinPull_e;

class digitalPin
{
public:
	digitalPin();
	digitalPin(pin_e pin,
			pinMode_e mode = PIN_MODE_OUTPUT,
			pinPull_e pull = PIN_PULL_NONE);
	void init(pin_e pin,
			pinMode_e mode = PIN_MODE_OUTPUT,
			pinPull_e pull = PIN_PULL_NONE);
	void digitalWrite(bool state);
	bool digitalRead();
	void toggle();
	pin_e getPin() {return _pin };
	~digitalPin();
private:
	pin_e _pin;
	static uint8_t _pinsActive;
};




class digitalIn : private digitalPin
{
public:
	digitalIn();
	digitalIn(pin_e pin, pinPull_e pull = PIN_PULL_NONE);
	void init(pin_e pin, pinPull_e pull = PIN_PULL_NONE);
	virtual bool digitalRead() = 0;
	uint8_t activeInTotal() { return _pinsActive; }
	~digitalIn() { _pinsActive--; };
private:
	uint8_t _btnNum;
	static uint8_t _pinsActive;
};




class digitalOut : private digitalPin
{
public:
	digitalOut();
	digitalOut(pin_e pin, pinPull_e pull = PIN_PULL_NONE);
	void init(pin_e pin, pinPull_e pull = PIN_PULL_NONE);
	virtual void digitalWrite(bool state) = 0;
	virtual void toggle() = 0;
	~digitalOut() { _pinsActive--; };
	uint8_t activeInTotal() { return _pinsActive; }
private:
	uint8_t _btnNum;
	static uint8_t _pinsActive;
};

#endif /* FIB_NCO_IO_IO_HPP_ */
