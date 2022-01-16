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

#include "fibstd/cast.hpp"
#include "system.hpp"

#include "mapbox/eternal.hpp"

#include "magic_enum.hpp"
#include "stm32f3xx_hal.h"
#include <algorithm>
#include <map>

#include <functional>

class Gpio
{
public:
    enum class Pin
    {
        // clang-format off
        A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15,
        B0, B1, B2, B3, B4, B5, B6, B7, B8, B9, B10, B11, B12, B13, B14, B15,
        C0, C1, C2, C3, C4, C5, C6, C7, C8, C9, C10, C11, C12, C13, C14, C15,
        D0, D1, D2, D3, D4, D5, D6, D7, D8, D9, D10, D11, D12, D13, D14, D15,
        // clang-format on
    };

    enum class PinState
    {
        low,
        high
    };

    enum class Pull
    {
        none,
        up,
        down
    };

    template <Pin pin> static void initAsInput()
    {
        Gpio::hwInit<pin>(Mode::input, Pull::none);
    };

    template <Pin pin> inline static void initAsOutput(PinState initial, Pull pull)
    {
        Gpio::write<pin>(initial);
        Gpio::hwInit<pin>(Mode::output, pull);
    };

    template <Pin pin>[[nodiscard]] static inline bool read()
    {
        return static_cast<bool>(HAL_GPIO_ReadPin(pinDescrOf<pin>().hwPort, pinDescrOf<pin>().hwPin));
    };

    template <Pin pin> static inline void write(PinState pinState)
    {
        HAL_GPIO_WritePin(pinDescrOf<pin>().hwPort, pinDescrOf<pin>().hwPin, getPinState(pinState));
    };

    template <Pin pin> static inline void toggle()
    {
        HAL_GPIO_TogglePin(pinDescrOf<pin>().hwPort, pinDescrOf<pin>().hwPin);
    }

    template <Pin pin> static void deinit()
    {
        HAL_GPIO_DeInit(pinDescrOf<pin>().hwPort, pinDescrOf<pin>().hwPin);
        // note gpio related clock still remain enabled
    };

    /** @brief lock pin hardware configuration registers until mcu reboot */
    template <Pin pin> static void lock()
    {
        HAL_GPIO_LockPin(pinDescrOf<pin>().hwPort, pinDescrOf<pin>().hwPin);
    }

protected:
    Gpio() = delete;

private:
    enum class Mode
    {
        input,
        output
    };

    static void preInitPortA()
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();
    }
    static void preInitPortB()
    {
        __HAL_RCC_GPIOB_CLK_ENABLE();
    }
    static void preInitPortC()
    {
        __HAL_RCC_GPIOC_CLK_ENABLE();
    }
    static void preInitPortD()
    {
        __HAL_RCC_GPIOD_CLK_ENABLE();
    }

    struct PinDescr
    {
        GPIO_TypeDef *hwPort;
        std::uint16_t hwPin;
        void (*preInitFunction)();
    };

    static constexpr const auto pinDescrs = mapbox::eternal::map<Pin, PinDescr>({
        {Pin::A0, {GPIOA, GPIO_PIN_0, preInitPortA}},   /* */
        {Pin::A1, {GPIOA, GPIO_PIN_1, preInitPortA}},   /* */
        {Pin::A2, {GPIOA, GPIO_PIN_2, preInitPortA}},   /* */
        {Pin::A3, {GPIOA, GPIO_PIN_3, preInitPortA}},   /* */
        {Pin::A4, {GPIOA, GPIO_PIN_4, preInitPortA}},   /* */
        {Pin::A5, {GPIOA, GPIO_PIN_5, preInitPortA}},   /* */
        {Pin::A6, {GPIOA, GPIO_PIN_6, preInitPortA}},   /* */
        {Pin::A7, {GPIOA, GPIO_PIN_7, preInitPortA}},   /* */
        {Pin::A8, {GPIOA, GPIO_PIN_8, preInitPortA}},   /* */
        {Pin::A9, {GPIOA, GPIO_PIN_9, preInitPortA}},   /* */
        {Pin::A10, {GPIOA, GPIO_PIN_10, preInitPortA}}, /* */
        {Pin::A11, {GPIOA, GPIO_PIN_11, preInitPortA}}, /* */
        {Pin::A12, {GPIOA, GPIO_PIN_12, preInitPortA}}, /* */
        {Pin::A13, {GPIOA, GPIO_PIN_13, preInitPortA}}, /* */
        {Pin::A14, {GPIOA, GPIO_PIN_14, preInitPortA}}, /* */
        {Pin::A15, {GPIOA, GPIO_PIN_15, preInitPortA}}, /* */
        {Pin::B0, {GPIOB, GPIO_PIN_0, preInitPortB}},   /* */
        {Pin::B1, {GPIOB, GPIO_PIN_1, preInitPortB}},   /* */
        {Pin::B2, {GPIOB, GPIO_PIN_2, preInitPortB}},   /* */
        {Pin::B3, {GPIOB, GPIO_PIN_3, preInitPortB}},   /* */
        {Pin::B4, {GPIOB, GPIO_PIN_4, preInitPortB}},   /* */
        {Pin::B5, {GPIOB, GPIO_PIN_5, preInitPortB}},   /* */
        {Pin::B6, {GPIOB, GPIO_PIN_6, preInitPortB}},   /* */
        {Pin::B7, {GPIOB, GPIO_PIN_7, preInitPortB}},   /* */
        {Pin::B8, {GPIOB, GPIO_PIN_8, preInitPortB}},   /* */
        {Pin::B9, {GPIOB, GPIO_PIN_9, preInitPortB}},   /* */
        {Pin::B10, {GPIOB, GPIO_PIN_10, preInitPortB}}, /* */
        {Pin::B11, {GPIOB, GPIO_PIN_11, preInitPortB}}, /* */
        {Pin::B12, {GPIOB, GPIO_PIN_12, preInitPortB}}, /* */
        {Pin::B13, {GPIOB, GPIO_PIN_13, preInitPortB}}, /* */
        {Pin::B14, {GPIOB, GPIO_PIN_14, preInitPortB}}, /* */
        {Pin::B15, {GPIOB, GPIO_PIN_15, preInitPortB}}, /* */
        {Pin::C0, {GPIOC, GPIO_PIN_0, preInitPortC}},   /* */
        {Pin::C1, {GPIOC, GPIO_PIN_1, preInitPortC}},   /* */
        {Pin::C2, {GPIOC, GPIO_PIN_2, preInitPortC}},   /* */
        {Pin::C3, {GPIOC, GPIO_PIN_3, preInitPortC}},   /* */
        {Pin::C4, {GPIOC, GPIO_PIN_4, preInitPortC}},   /* */
        {Pin::C5, {GPIOC, GPIO_PIN_5, preInitPortC}},   /* */
        {Pin::C6, {GPIOC, GPIO_PIN_6, preInitPortC}},   /* */
        {Pin::C7, {GPIOC, GPIO_PIN_7, preInitPortC}},   /* */
        {Pin::C8, {GPIOC, GPIO_PIN_8, preInitPortC}},   /* */
        {Pin::C9, {GPIOC, GPIO_PIN_9, preInitPortC}},   /* */
        {Pin::C10, {GPIOC, GPIO_PIN_10, preInitPortC}}, /* */
        {Pin::C11, {GPIOC, GPIO_PIN_11, preInitPortC}}, /* */
        {Pin::C12, {GPIOC, GPIO_PIN_12, preInitPortC}}, /* */
        {Pin::C13, {GPIOC, GPIO_PIN_13, preInitPortC}}, /* */
        {Pin::C14, {GPIOC, GPIO_PIN_14, preInitPortC}}, /* OSC32_IN */
        {Pin::C15, {GPIOC, GPIO_PIN_15, preInitPortC}}, /* OSC32_OUT */
        {Pin::D0, {GPIOD, GPIO_PIN_0, preInitPortD}},   /* na */
        {Pin::D1, {GPIOD, GPIO_PIN_1, preInitPortD}},   /* na */
        {Pin::D2, {GPIOD, GPIO_PIN_2, preInitPortD}},   /* */
        {Pin::D3, {GPIOD, GPIO_PIN_3, preInitPortD}},   /* na */
        {Pin::D4, {GPIOD, GPIO_PIN_4, preInitPortD}},   /* na */
        {Pin::D5, {GPIOD, GPIO_PIN_5, preInitPortD}},   /* na */
        {Pin::D6, {GPIOD, GPIO_PIN_6, preInitPortD}},   /* na */
        {Pin::D7, {GPIOD, GPIO_PIN_7, preInitPortD}},   /* na */
        {Pin::D8, {GPIOD, GPIO_PIN_8, preInitPortD}},   /* na */
        {Pin::D9, {GPIOD, GPIO_PIN_9, preInitPortD}},   /* na */
        {Pin::D10, {GPIOD, GPIO_PIN_10, preInitPortD}}, /* na */
        {Pin::D11, {GPIOD, GPIO_PIN_11, preInitPortD}}, /* na */
        {Pin::D12, {GPIOD, GPIO_PIN_12, preInitPortD}}, /* na */
        {Pin::D13, {GPIOD, GPIO_PIN_13, preInitPortD}}, /* na */
        {Pin::D14, {GPIOD, GPIO_PIN_14, preInitPortD}}, /* na */
        {Pin::D15, {GPIOD, GPIO_PIN_15, preInitPortD}}, /* na */
    });

    template <Pin pin> static constexpr PinDescr pinDescrOf()
    {
        constexpr auto pinDescr = pinDescrs.find(pin);
        if (pinDescr != pinDescrs.end())
        {
            return pinDescr->second;
        }
    }

    static constexpr uint32_t getMode(Gpio::Mode mode)
    {
        const uint32_t cMap[static_cast<std::size_t>(magic_enum::enum_count<Gpio::Mode>())] = {GPIO_MODE_INPUT,
                                                                                               GPIO_MODE_OUTPUT_PP};
        return cMap[static_cast<std::size_t>(mode)];
    }

    static constexpr uint32_t getPull(Gpio::Pull pull)
    {
        const uint32_t cMap[static_cast<std::size_t>(magic_enum::enum_count<Gpio::Pull>())] = {GPIO_NOPULL, GPIO_PULLUP,
                                                                                               GPIO_PULLDOWN};
        return cMap[static_cast<std::size_t>(pull)];
    }

    static constexpr GPIO_PinState getPinState(Gpio::PinState pinState)
    {
        const GPIO_PinState cMap[static_cast<std::size_t>(magic_enum::enum_count<Gpio::PinState>())] = {GPIO_PIN_RESET,
                                                                                                        GPIO_PIN_SET};
        return cMap[static_cast<std::size_t>(pinState)];
    }

    template <Pin pin> static void hwInit(Gpio::Mode mode, Gpio::Pull pull)
    {
        if (pinDescrOf<pin>().preInitFunction)
        {
            pinDescrOf<pin>().preInitFunction();
        }

        GPIO_InitTypeDef GPIO_InitStruct = {};
        GPIO_InitStruct.Pin = pinDescrOf<pin>().hwPin;
        GPIO_InitStruct.Mode = Gpio::getMode(mode);
        GPIO_InitStruct.Pull = Gpio::getPull(pull);
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(pinDescrOf<pin>().hwPort, &GPIO_InitStruct);
    }
};
