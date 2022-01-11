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

#include "system.hpp"

#include "magic_enum.hpp"
#include "stm32f3xx_hal.h"

#include <functional>

#define GPIO_PINS                                                                                                      \
    GPIO_PIN(A0, GPIO_PIN_0, GPIOA, preInitPortA)   /* */                                                              \
    GPIO_PIN(A1, GPIO_PIN_1, GPIOA, preInitPortA)   /* */                                                              \
    GPIO_PIN(A2, GPIO_PIN_2, GPIOA, preInitPortA)   /* */                                                              \
    GPIO_PIN(A3, GPIO_PIN_3, GPIOA, preInitPortA)   /* */                                                              \
    GPIO_PIN(A4, GPIO_PIN_4, GPIOA, preInitPortA)   /* */                                                              \
    GPIO_PIN(A5, GPIO_PIN_5, GPIOA, preInitPortA)   /* */                                                              \
    GPIO_PIN(A6, GPIO_PIN_6, GPIOA, preInitPortA)   /* */                                                              \
    GPIO_PIN(A7, GPIO_PIN_7, GPIOA, preInitPortA)   /* */                                                              \
    GPIO_PIN(A8, GPIO_PIN_8, GPIOA, preInitPortA)   /* */                                                              \
    GPIO_PIN(A9, GPIO_PIN_9, GPIOA, preInitPortA)   /* */                                                              \
    GPIO_PIN(A10, GPIO_PIN_10, GPIOA, preInitPortA) /* */                                                              \
    GPIO_PIN(A11, GPIO_PIN_11, GPIOA, preInitPortA) /* */                                                              \
    GPIO_PIN(A12, GPIO_PIN_12, GPIOA, preInitPortA) /* */                                                              \
    GPIO_PIN(A13, GPIO_PIN_13, GPIOA, preInitPortA) /* */                                                              \
    GPIO_PIN(A14, GPIO_PIN_14, GPIOA, preInitPortA) /* */                                                              \
    GPIO_PIN(A15, GPIO_PIN_15, GPIOA, preInitPortA) /* */                                                              \
    GPIO_PIN(B0, GPIO_PIN_0, GPIOB, preInitPortB)   /* */                                                              \
    GPIO_PIN(B1, GPIO_PIN_1, GPIOB, preInitPortB)   /* */                                                              \
    GPIO_PIN(B2, GPIO_PIN_2, GPIOB, preInitPortB)   /* */                                                              \
    GPIO_PIN(B3, GPIO_PIN_3, GPIOB, preInitPortB)   /* */                                                              \
    GPIO_PIN(B4, GPIO_PIN_4, GPIOB, preInitPortB)   /* */                                                              \
    GPIO_PIN(B5, GPIO_PIN_5, GPIOB, preInitPortB)   /* */                                                              \
    GPIO_PIN(B6, GPIO_PIN_6, GPIOB, preInitPortB)   /* */                                                              \
    GPIO_PIN(B7, GPIO_PIN_7, GPIOB, preInitPortB)   /* */                                                              \
    GPIO_PIN(B8, GPIO_PIN_8, GPIOB, preInitPortB)   /* */                                                              \
    GPIO_PIN(B9, GPIO_PIN_9, GPIOB, preInitPortB)   /* */                                                              \
    GPIO_PIN(B10, GPIO_PIN_10, GPIOB, preInitPortB) /* */                                                              \
    GPIO_PIN(B11, GPIO_PIN_11, GPIOB, preInitPortB) /* */                                                              \
    GPIO_PIN(B12, GPIO_PIN_12, GPIOB, preInitPortB) /* */                                                              \
    GPIO_PIN(B13, GPIO_PIN_13, GPIOB, preInitPortB) /* */                                                              \
    GPIO_PIN(B14, GPIO_PIN_14, GPIOB, preInitPortB) /* */                                                              \
    GPIO_PIN(B15, GPIO_PIN_15, GPIOB, preInitPortB) /* */                                                              \
    GPIO_PIN(C0, GPIO_PIN_0, GPIOC, preInitPortC)   /* */                                                              \
    GPIO_PIN(C1, GPIO_PIN_1, GPIOC, preInitPortC)   /* */                                                              \
    GPIO_PIN(C2, GPIO_PIN_2, GPIOC, preInitPortC)   /* */                                                              \
    GPIO_PIN(C3, GPIO_PIN_3, GPIOC, preInitPortC)   /* */                                                              \
    GPIO_PIN(C4, GPIO_PIN_4, GPIOC, preInitPortC)   /* */                                                              \
    GPIO_PIN(C5, GPIO_PIN_5, GPIOC, preInitPortC)   /* */                                                              \
    GPIO_PIN(C6, GPIO_PIN_6, GPIOC, preInitPortC)   /* */                                                              \
    GPIO_PIN(C7, GPIO_PIN_7, GPIOC, preInitPortC)   /* */                                                              \
    GPIO_PIN(C8, GPIO_PIN_8, GPIOC, preInitPortC)   /* */                                                              \
    GPIO_PIN(C9, GPIO_PIN_9, GPIOC, preInitPortC)   /* */                                                              \
    GPIO_PIN(C10, GPIO_PIN_10, GPIOC, preInitPortC) /* */                                                              \
    GPIO_PIN(C11, GPIO_PIN_11, GPIOC, preInitPortC) /* */                                                              \
    GPIO_PIN(C12, GPIO_PIN_12, GPIOC, preInitPortC) /* */                                                              \
    GPIO_PIN(C13, GPIO_PIN_13, GPIOC, preInitPortC) /* */                                                              \
    GPIO_PIN(C14, GPIO_PIN_14, GPIOC, preInitPortC) /* OSC32_IN */                                                     \
    GPIO_PIN(C15, GPIO_PIN_15, GPIOC, preInitPortC) /* OSC32_OUT */                                                    \
    GPIO_PIN(D0, GPIO_PIN_0, GPIOD, preInitPortD)   /* N/A */                                                          \
    GPIO_PIN(D1, GPIO_PIN_1, GPIOD, preInitPortD)   /* N/A */                                                          \
    GPIO_PIN(D2, GPIO_PIN_2, GPIOD, preInitPortD)   /* */                                                              \
    GPIO_PIN(D3, GPIO_PIN_3, GPIOD, preInitPortD)   /* N/A */                                                          \
    GPIO_PIN(D4, GPIO_PIN_4, GPIOD, preInitPortD)   /* N/A */                                                          \
    GPIO_PIN(D5, GPIO_PIN_5, GPIOD, preInitPortD)   /* N/A */                                                          \
    GPIO_PIN(D6, GPIO_PIN_6, GPIOD, preInitPortD)   /* N/A */                                                          \
    GPIO_PIN(D7, GPIO_PIN_7, GPIOD, preInitPortD)   /* N/A */                                                          \
    GPIO_PIN(D8, GPIO_PIN_8, GPIOD, preInitPortD)   /* N/A */                                                          \
    GPIO_PIN(D9, GPIO_PIN_9, GPIOD, preInitPortD)   /* N/A */                                                          \
    GPIO_PIN(D10, GPIO_PIN_10, GPIOD, preInitPortD) /* N/A */                                                          \
    GPIO_PIN(D11, GPIO_PIN_11, GPIOD, preInitPortD) /* N/A */                                                          \
    GPIO_PIN(D12, GPIO_PIN_12, GPIOD, preInitPortD) /* N/A */                                                          \
    GPIO_PIN(D13, GPIO_PIN_13, GPIOD, preInitPortD) /* N/A */                                                          \
    GPIO_PIN(D14, GPIO_PIN_14, GPIOD, preInitPortD) /* N/A */                                                          \
    GPIO_PIN(D15, GPIO_PIN_15, GPIOD, preInitPortD) /* N/A */

class Gpio
{
public:
    enum class Pin
    {
#define GPIO_PIN(enumName, hwPin, hwPort, preInitFunction) enumName,
        GPIO_PINS
#undef GPIO_PIN
    };

    enum class Mode
    {
        input,
        output
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

    template <Pin pin> static void init(Mode mode, PinState pinStateInitial, Pull pull) // override
    {
        Gpio::write<pin>(pinStateInitial);
        Gpio::hwInit<pin>(mode, pull);
    };

    template <Pin pin>[[nodiscard]] static inline bool read() // override
    {
        return static_cast<bool>(HAL_GPIO_ReadPin(getHwPort<pin>(), getHwPin<pin>()));
    };

    template <Pin pin> static inline void write(PinState pinState) // override
    {
        HAL_GPIO_WritePin(getHwPort<pin>(), getHwPin<pin>(), getPinState(pinState));
    };

    template <Pin pin> static inline void toggle()
    {
        HAL_GPIO_TogglePin(getHwPort<pin>(), getHwPin<pin>());
    }

    template <Pin pin> static void deinit() // override
    {
        HAL_GPIO_DeInit(getHwPort<pin>(), getHwPin<pin>());
        // note gpio related clock still remain enabled
    };

    /** @brief lock pin hardware configuration registers until mcu reboot */
    template <Pin pin> static void lock()
    {
        HAL_GPIO_LockPin(getHwPort<pin>(), getHwPin<pin>());
    }

protected:
    Gpio() = delete;

private:
    template <Pin pin> static constexpr uint16_t getHwPin()
    {
        if constexpr (false)
        {
        }
#define GPIO_PIN(enumName, hwPin, hwPort, preInitFunction)                                                             \
    else if constexpr (pin == Pin::enumName)                                                                           \
    {                                                                                                                  \
        return hwPin;                                                                                                  \
    }
        GPIO_PINS
#undef GPIO_PIN
    }
    template <Pin pin> static constexpr GPIO_TypeDef *getHwPort()
    {
        if constexpr (false)
        {
        }
#define GPIO_PIN(enumName, hwPin, hwPort, preInitFunction)                                                             \
    else if constexpr (pin == Pin::enumName)                                                                           \
    {                                                                                                                  \
        return hwPort;                                                                                                 \
    }
        GPIO_PINS
#undef GPIO_PIN
    }
    template <Pin pin> static constexpr std::function<void()> getHwPreInitFunction()
    {
        if constexpr (false)
        {
        }
#define GPIO_PIN(enumName, hwPin, hwPort, preInitFunction)                                                             \
    else if constexpr (pin == Pin::enumName)                                                                           \
    {                                                                                                                  \
        return preInitFunction;                                                                                        \
    }
        GPIO_PINS
#undef GPIO_PIN
    }

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

    static constexpr uint32_t getMode(Gpio::Mode mode)
    {
        const uint32_t cMap[static_cast<std::size_t>(magic_enum::enum_count<Gpio::Mode>()) + 1] = {
            GPIO_MODE_INPUT, GPIO_MODE_OUTPUT_PP, GPIO_MODE_OUTPUT_OD};
        return cMap[static_cast<std::size_t>(mode)];
    }

    static constexpr uint32_t getPull(Gpio::Pull pull)
    {
        const uint32_t cMap[static_cast<std::size_t>(magic_enum::enum_count<Gpio::Pull>()) + 1] = {
            GPIO_NOPULL, GPIO_PULLUP, GPIO_PULLDOWN};
        return cMap[static_cast<std::size_t>(pull)];
    }

    static constexpr GPIO_PinState getPinState(Gpio::PinState pinState)
    {
        const GPIO_PinState cMap[static_cast<std::size_t>(magic_enum::enum_count<Gpio::PinState>()) + 1] = {
            GPIO_PIN_RESET, GPIO_PIN_SET};
        return cMap[static_cast<std::size_t>(pinState)];
    }
    template <Pin pin> static void hwInit(Gpio::Mode mode, Gpio::Pull pull)
    {
        if (getHwPreInitFunction<pin>())
        {
            getHwPreInitFunction<pin>();
        }

        GPIO_InitTypeDef GPIO_InitStruct = {};
        GPIO_InitStruct.Pin = getHwPin<pin>();
        GPIO_InitStruct.Mode = Gpio::getMode(mode);
        GPIO_InitStruct.Pull = Gpio::getPull(pull);
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(getHwPort<pin>(), &GPIO_InitStruct);
    }
};
