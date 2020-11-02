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

// map out arbitrary alphabetic pins to your hardware pins
const struct {GPIO_TypeDef * pPort; uint16_t pinNum;} pinsMap[] =
{
	{ nullptr, 0}, 			// is PIN_UNKNOWN (undefined)
	{ GPIOA, GPIO_PIN_0},	// is PIN_A
	{ GPIOA, GPIO_PIN_1},	// is PIN_B
	{ GPIOA, GPIO_PIN_2},	// is PIN_C
	{ GPIOA, GPIO_PIN_3},	// is PIN_D

	{ GPIOA, GPIO_PIN_4},	// is PIN_E
	{ GPIOA, GPIO_PIN_5},	// is PIN_F
	{ GPIOA, GPIO_PIN_6},	// is PIN_G
	{ GPIOA, GPIO_PIN_7},	// is PIN_H

	{ GPIOA, GPIO_PIN_8},	// is PIN_I
	{ GPIOA, GPIO_PIN_9},	// is PIN_J
	{ GPIOA, GPIO_PIN_10},	// is PIN_K
	{ GPIOA, GPIO_PIN_11},	// is PIN_L

	{ GPIOA, GPIO_PIN_12},	// is PIN_M
	// no board pin for A13
	// no board pin for A14
	{ GPIOA, GPIO_PIN_15},	// is PIN_N


	{ GPIOB, GPIO_PIN_0},	// is PIN_O
	{ GPIOB, GPIO_PIN_1},	// is PIN_P
	// no board pin for B2
	{ GPIOB, GPIO_PIN_3},	// is PIN_Q

	{ GPIOB, GPIO_PIN_4},	// is PIN_R
	{ GPIOB, GPIO_PIN_5},	// is PIN_S
	{ GPIOB, GPIO_PIN_6},	// is PIN_T
	{ GPIOB, GPIO_PIN_7},	// is PIN_U

	{ GPIOB, GPIO_PIN_8},	// is PIN_V
	{ GPIOB, GPIO_PIN_9},	// is PIN_W
	{ GPIOB, GPIO_PIN_10},	// is PIN_X
	{ GPIOB, GPIO_PIN_11},	// is PIN_Y

	{ GPIOB, GPIO_PIN_12},	// is PIN_Z
	{ GPIOB, GPIO_PIN_13},	// is PIN_AA
	{ GPIOB, GPIO_PIN_14},	// is PIN_AB
	{ GPIOB, GPIO_PIN_15},	// is PIN_AC


	{ GPIOC, GPIO_PIN_13},	// is PIN_AD
	{ GPIOC, GPIO_PIN_14},	// is PIN_AE
	{ GPIOC, GPIO_PIN_15},	// is PIN_AF
};


void GPIO::digitalWrite(bool state)
{
	if(_pin != PIN_UNKNOWN && _initialized == true/* && ????*/)
	{
		HAL_GPIO_WritePin(pinsMap[_pin].pPort, 
			pinsMap[_pin].pinNum, (GPIO_PinState)state);

	}
}


bool GPIO::digitalRead()
{
	return (bool)HAL_GPIO_ReadPin(pinsMap[_pin].pPort, pinsMap[_pin].pinNum);
}


void GPIO::toggle()
{
	HAL_GPIO_TogglePin(pinsMap[_pin].pPort, pinsMap[_pin].pinNum);
}


void GPIO::init(pin_e pin, pinMode_e mode, PinPull pull) 
{	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();

	digitalWrite(false); //default level

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	GPIO_InitStruct.Pin = pinsMap[_pin].pinNum;
	GPIO_InitStruct.Mode = mode;
	GPIO_InitStruct.Pull = pull;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(pinsMap[_pin].pPort, &GPIO_InitStruct);
	_pinsActive++;
	_initialized = true;
}


GPIO::GPIO(pin_e pin, pinMode_e mode, PinPull pull) 
	: _pin(pin), _initialized(false)
{
	init(pin, mode, pull);
}


GPIO::~GPIO()
{
	_initialized = false;
	_pinsActive--;
}

} //namespace Hardware
