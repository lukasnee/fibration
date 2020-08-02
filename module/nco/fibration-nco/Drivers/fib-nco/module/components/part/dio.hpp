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

#include "gpio.hpp"

class digitalIn : private digitalPin
{
public:
	digitalIn(pins_e pin) : digitalPin(pin) { _btnNum = _pinsActive; _pinsActive++; };
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
	digitalOut() { _btnNum = _pinsActive; _pinsActive++; };
	virtual void digitalWrite(bool state) = 0;
	virtual void toggle() = 0;
	~digitalOut() { _pinsActive--; };
	uint8_t activeInTotal() { return _pinsActive; }
private:
	uint8_t _btnNum;
	static uint8_t _pinsActive;
};


#endif /* FIB_NCO_IO_IO_HPP_ */
