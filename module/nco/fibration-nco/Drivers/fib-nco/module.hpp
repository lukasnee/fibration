/*
    A general module class for modular synth project Fibration.
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

#include "stdint.h"

namespace FIB {

////////////////////////////////////////////////////////////////////////////////
// Hardware wrapper
////////////////////////////////////////////////////////////////////////////////

#include "main.h"

extern ADC_HandleTypeDef hadc1;
extern DAC_HandleTypeDef hdac;
extern UART_HandleTypeDef huart1;

typedef struct pin_
{
	GPIO_TypeDef * pPort;
	uint16_t pinNum;
} pin_t;

class digitalIn
{
	digitalIn()
	{
		_btnNum = _pinsActive;
		_pinsActive++;
		/* already initialized in main.c autogen code*/
	};

	~digitalIn()
	{
		_pinsActive--;
	};

	bool read()
	{
		return (bool)HAL_GPIO_ReadPin(InputPinLUT[_btnNum].pPort, InputPinLUT[_btnNum].pinNum);
	}

	uint8_t _btnNum;

	static const pin_t InputPinLUT[];
	static uint8_t _pinsActive;
};

class digitalOut
{
	digitalOut()
	{
		_btnNum = _pinsActive;
		_pinsActive++;
		/* already initialized in main.c autogen code*/
	};

	~digitalOut()
	{
		_pinsActive--;
	};

	void write(bool state)
	{
		HAL_GPIO_WritePin(OutputPinLUT[_btnNum].pPort, OutputPinLUT[_btnNum].pinNum,
				(GPIO_PinState)state);
	}

	void toggle()
	{
		HAL_GPIO_TogglePin(OutputPinLUT[_btnNum].pPort, OutputPinLUT[_btnNum].pinNum);
	}

	uint8_t activeInTotal()
	{
		return _pinsActive;
	}

	uint8_t _btnNum;

	static const pin_t OutputPinLUT[];
	static uint8_t _pinsActive;
};

class ADC
{

};

class Serial
{


};

class DAC
{

};

////////////////////////////////////////////////////////////////////////////////
// Module I/O
////////////////////////////////////////////////////////////////////////////////

class Button {

	digitalIn dIn;

};

class LED {

	digitalOut dOut;

	void turnOn() {

		dOut.setHigh();
	}

	void turnOff() {

		dOut.setHigh();
	}

	void toggle(void) {

		dOut.toggle();
	}

};

class Potentiometer {


};

class Slider {


}

class Switch {


};

class SigIn : public ADC {



};

class SigOut : public DAC {



};

////////////////////////////////////////////////////////////////////////////////
// Module
////////////////////////////////////////////////////////////////////////////////


class Module {


};

} // namespace FIB

