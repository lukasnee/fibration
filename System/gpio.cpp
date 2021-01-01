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

#include "gpio.hpp"
#include "system.hpp"

#include "stm32f3xx_hal.h"

static constexpr GPIO_TypeDef * sGetPort(Gpio::Port port)
{
    const GPIO_TypeDef * cMap[Gpio::Port::_PORT_ENUM_MAX + 1] = {
        GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF
    };
    return const_cast<GPIO_TypeDef *>(cMap[port]);
}

static constexpr uint16_t sGetPin(Gpio::Pin pin)
{
    const uint16_t cMap[Gpio::Pin::_PIN_ENUM_MAX + 1] = {
        GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3,
        GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7,
        GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10, GPIO_PIN_11,
        GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15
    };
    return cMap[pin];
}

static constexpr uint32_t sGetMode(Gpio::Mode mode)
{
    const uint32_t cMap[Gpio::Mode::_MODE_ENUM_MAX + 1] = {
        GPIO_MODE_INPUT,
        GPIO_MODE_OUTPUT_PP,
        GPIO_MODE_OUTPUT_OD
    };
    return cMap[mode];
}

static constexpr uint32_t sGetPull(Gpio::Pull pull)
{
    const uint32_t cMap[Gpio::Pull::_PULL_ENUM_MAX + 1] = {
        GPIO_NOPULL,
        GPIO_PULLUP,
        GPIO_PULLDOWN
    };
    return cMap[pull];
}

static constexpr uint32_t sGetSpeed(Gpio::Speed speed)
{
    const uint32_t cMap[Gpio::Speed::_SPEED_ENUM_MAX + 1] = {
        GPIO_SPEED_FREQ_LOW,
        GPIO_SPEED_FREQ_MEDIUM,
        GPIO_SPEED_FREQ_HIGH
    };
    return cMap[speed];
}

static constexpr GPIO_PinState sGetPinState(Gpio::PinState pinState)
{
    const GPIO_PinState cMap[Gpio::PinState::_PINSTATE_ENUM_MAX + 1] = {
        GPIO_PIN_RESET,
        GPIO_PIN_SET
    };
    return cMap[pinState];
}

static void sEnableGpioClock(Gpio::Port port)
{
    switch(port)
    {
        case Gpio::Port::A: __HAL_RCC_GPIOA_CLK_ENABLE(); break;
        case Gpio::Port::B: __HAL_RCC_GPIOB_CLK_ENABLE(); break;
        case Gpio::Port::C: __HAL_RCC_GPIOC_CLK_ENABLE(); break;
        case Gpio::Port::D: __HAL_RCC_GPIOD_CLK_ENABLE(); break;
        case Gpio::Port::F: __HAL_RCC_GPIOF_CLK_ENABLE(); break;
        default: break;
    }
}

static void sGpioInit(
    Gpio::Port port,
    Gpio::Pin pin,
    Gpio::Mode mode,
    Gpio::Pull pull,
    Gpio::Speed speed)
{
    sEnableGpioClock(port);

    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    GPIO_InitStruct.Pin = sGetPin(pin);
    GPIO_InitStruct.Mode = sGetMode(mode);
    GPIO_InitStruct.Pull = sGetPull(pull);
    GPIO_InitStruct.Speed = sGetSpeed(speed);
    HAL_GPIO_Init(sGetPort(port), &GPIO_InitStruct);
}



Gpio::Gpio(Port port, Pin pin) : 
    port(port),
    pin(pin),
    isInitialized(false)
{
}

void Gpio::init(Mode mode, Pull pull, Speed speed)
{
    sGpioInit(this->port, this->pin, mode, pull, speed);
    isInitialized = true;
}

void Gpio::init(Mode mode, PinState initial, Pull pull, Speed speed)
{
    write(initial);
    init(mode, pull, speed);
}


bool Gpio::read()
{
    return (bool)HAL_GPIO_ReadPin(sGetPort(this->port), sGetPin(this->pin));
}


void Gpio::write(PinState value)
{
    HAL_GPIO_WritePin(sGetPort(this->port), sGetPin(this->pin), sGetPinState(value));
}


void Gpio::toggle()
{
    HAL_GPIO_TogglePin(sGetPort(this->port), sGetPin(this->pin));
}


void Gpio::lock()
{
    HAL_GPIO_LockPin(sGetPort(this->port), sGetPin(this->pin));
}


void Gpio::deinit()
{
    HAL_GPIO_DeInit(sGetPort(this->port), sGetPin(this->pin));
    // note gpio related clock still remain enabled
}


Gpio::~Gpio()
{
    deinit();
}
