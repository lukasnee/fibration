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

namespace FIB {

////////////////////////////////////////////////////////////////////////////////
// Module
////////////////////////////////////////////////////////////////////////////////


class Module {


};

////////////////////////////////////////////////////////////////////////////////
// Module I/O
////////////////////////////////////////////////////////////////////////////////

class Button {

	digitalIn dIn;

};

class LED {

	digitalOut dOut;

	void turnOn(void) {

		dOut.setHigh();
	}

	void turnOff(void) {

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
// Hardware wrapper
////////////////////////////////////////////////////////////////////////////////

#include "main.h"

extern ADC_HandleTypeDef hadc1;
extern DAC_HandleTypeDef hdac;
extern UART_HandleTypeDef huart1;

class digitalIn
{

	bool read(void)
	{
		return (bool)HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);
	}

	GPIO_TypeDef * pPort;
	uint16_t pin;
};

class digitalOut
{

	digitalOut() { /* gets initialized in main by cubeide autogen code */ };

	void write(bool state)
	{
		HAL_GPIO_WritePin(pPort, pin, (GPIO_PinState)state);
	}

	void toggle(void)
	{
		HAL_GPIO_TogglePin(pPort, pin);
	}

	GPIO_TypeDef * pPort;
	uint16_t pin;

};
class ADC
{

};

class Serial
{


};

} // namespace FIB


class DAC {

};
