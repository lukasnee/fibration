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

#include "common.hpp"

#include "stm32f3xx_hal.h"
#include "gpio.hpp"

namespace Hardware {

uint8_t GPIO::_pinsActive = 0;

typedef struct PinData_ 
{
	GPIO_TypeDef * pPort; 
	uint16_t pinNum;
} PinData;

// map out arbitrary alphabetic pins to your hardware pins
const PinData cPinsMap[] =
{
	//{ nullptr, 0}, 		// is PIN_UNKNOWN (undefined)

	{ GPIOA, GPIO_PIN_0},	// is PIN_0
	{ GPIOA, GPIO_PIN_1},	// is PIN_1
	{ GPIOA, GPIO_PIN_2},	// is PIN_2
	{ GPIOA, GPIO_PIN_3},	// is PIN_3
	{ GPIOA, GPIO_PIN_4},	// is PIN_4
	{ GPIOA, GPIO_PIN_5},	// is PIN_5
	{ GPIOA, GPIO_PIN_6},	// is PIN_6
	{ GPIOA, GPIO_PIN_7},	// is PIN_7
	{ GPIOA, GPIO_PIN_8},	// is PIN_8
	{ GPIOA, GPIO_PIN_9},	// is PIN_9
	{ GPIOA, GPIO_PIN_10},	// is PIN_A
	{ GPIOA, GPIO_PIN_11},	// is PIN_B
	{ GPIOA, GPIO_PIN_12},	// is PIN_C
// 	{ GPIOA, GPIO_PIN_13},	// is PIN_D used for SWIO
// 	{ GPIOA, GPIO_PIN_14},	// is PIN_E used for SWCLK
	{ GPIOA, GPIO_PIN_15}, 	// is PIN_F

	{ GPIOB, GPIO_PIN_0},	// is PIN_10
	{ GPIOB, GPIO_PIN_1},	// is PIN_11
//	{ GPIOB, GPIO_PIN_2}, // is PIN_12 used for BOOT1
	{ GPIOB, GPIO_PIN_3},	// is PIN_13
	{ GPIOB, GPIO_PIN_4},	// is PIN_14
	{ GPIOB, GPIO_PIN_5},	// is PIN_15
	{ GPIOB, GPIO_PIN_6},	// is PIN_16
	{ GPIOB, GPIO_PIN_7},	// is PIN_17
	{ GPIOB, GPIO_PIN_8},	// is PIN_18
	{ GPIOB, GPIO_PIN_9},	// is PIN_19
	{ GPIOB, GPIO_PIN_10},	// is PIN_1A
	{ GPIOB, GPIO_PIN_11},	// is PIN_1B
	{ GPIOB, GPIO_PIN_12},	// is PIN_1C
	{ GPIOB, GPIO_PIN_13},	// is PIN_1D
	{ GPIOB, GPIO_PIN_14},	// is PIN_1E
	{ GPIOB, GPIO_PIN_15},	// is PIN_1F

	{ GPIOC, GPIO_PIN_13},	// is PIN_2C, RobotDyn LED pin, active LOW
	{ GPIOC, GPIO_PIN_14},	// is PIN_2D
	{ GPIOC, GPIO_PIN_15},	// is PIN_2E
};


void GPIO::digitalWrite(bool state)
{
	HAL_GPIO_WritePin(
		cPinsMap[_pin].pPort, 
		cPinsMap[_pin].pinNum, 
		state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}


bool GPIO::digitalRead()
{
	return (bool)HAL_GPIO_ReadPin(cPinsMap[_pin].pPort, cPinsMap[_pin].pinNum);
}


void GPIO::toggle()
{
	HAL_GPIO_TogglePin(cPinsMap[_pin].pPort, cPinsMap[_pin].pinNum);
}


void GPIO::init(pin_e pin, bool defaultState, pinMode_e mode, PinPull pull) 
{	
	if(pin < sizeof(cPinsMap)/sizeof(PinData))
	{
		switch((uint32_t)cPinsMap[pin].pPort)
		{
			case GPIOA_BASE: __HAL_RCC_GPIOA_CLK_ENABLE(); break;
			case GPIOB_BASE: __HAL_RCC_GPIOB_CLK_ENABLE(); break;
			case GPIOC_BASE: __HAL_RCC_GPIOC_CLK_ENABLE(); break;
			case GPIOF_BASE: __HAL_RCC_GPIOF_CLK_ENABLE(); break;
		}

		digitalWrite(defaultState);

		GPIO_InitTypeDef GPIO_InitStruct = { 0 };
		GPIO_InitStruct.Pin = cPinsMap[_pin].pinNum;
		GPIO_InitStruct.Mode = mode;
		GPIO_InitStruct.Pull = pull;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
		HAL_GPIO_Init(cPinsMap[_pin].pPort, &GPIO_InitStruct);

		_pinsActive++;
		_initialized = true;
	}
}


GPIO::GPIO(pin_e pin, bool defaultState, pinMode_e mode, PinPull pull) 
	: _pin(pin), _initialized(false)
{
	init(pin, defaultState, mode, pull);
}

void GPIO::deinit() 
{
	HAL_GPIO_DeInit(cPinsMap[_pin].pPort, cPinsMap[_pin].pinNum);	
		_initialized = false;
	_pinsActive--;
}

GPIO::~GPIO()
{
	deinit();
}

} //namespace Hardware
