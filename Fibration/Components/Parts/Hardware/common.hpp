/*
    common hardware header for modular synth project Fibration.
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

#ifndef FIB_NCO_MODULE_COMPONENTS_HARDWARE_HARDWARE_HPP_
#define FIB_NCO_MODULE_COMPONENTS_HARDWARE_HARDWARE_HPP_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f3xx_hal.h"

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

#ifdef __cplusplus
}
#endif


#endif /* FIB_NCO_MODULE_COMPONENTS_HARDWARE_HARDWARE_HPP_ */
