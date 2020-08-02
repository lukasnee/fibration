/*
    GPIO class for modular synth project Fibration.
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

#include "../../components/hardware/gpio.hpp"

#include "../../components/hardware/hardware.hpp"

uint8_t digitalPin::_pinsActive = 0;

// map out arbitrary alphabetic pins to your hardware pins
const struct {GPIO_TypeDef * pPort; uint16_t pinNum;} pinsMap[] =
{
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

void digitalPin::digitalWrite(bool state)
{
	HAL_GPIO_WritePin(pinsMap[_pin].pPort, pinsMap[_pin].pinNum, (GPIO_PinState)state);
};

bool digitalPin::digitalRead()
{
	return (bool)HAL_GPIO_ReadPin(pinsMap[_pin].pPort, pinsMap[_pin].pinNum);
};

void digitalPin::toggle()
{
	HAL_GPIO_TogglePin(pinsMap[_pin].pPort, pinsMap[_pin].pinNum);
};

digitalPin::digitalPin(pins_e pin) : _pin(pin)
{
	_pinsActive++;
	/* initialization done in main.c by cubeide autogen code */
};
digitalPin::~digitalPin()
{
	_pinsActive--;
};








/*	RobotDyn STM32F303 dev board pinout

	GPIO PORT A
		PIN_A0, 	PIN_A1,		PIN_A2,		PIN_A3,
		PIN_A4,		PIN_A5,		PIN_A6,		PIN_A7,
		PIN_A8,		PIN_A9,		PIN_A10,	PIN_A11,
		PIN_A12,							PIN_A15,
	GPIO PORT B
		PIN_B0,		PIN_B1,					PIN_B3,
		PIN_B4,		PIN_B5,		PIN_B6, 	PIN_B7,
		PIN_B8,		PIN_B9,		PIN_B10,	PIN_B11,
		PIN_B12,	PIN_B13,	PIN_B14,	PIN_B15,
	GPIO PORT C
		PIN_C13,	PIN_C14,	PIN_B15,
*/

//
//const pin_t buttonLUT[1] =
//{
//	{GPIOC, GPIO_PIN_13}
//};
//
//const pin_t sigInLUT[] =
//{
//};
//
//const pin_t sigOutLUT[] =
//{
//};
//
//const pin_t OutputPinLUT[1] =
//{
//	{GPIOA, GPIO_PIN_12}
//};
//
