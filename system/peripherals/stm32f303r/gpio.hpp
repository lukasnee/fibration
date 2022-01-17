#pragma once

#include "magic_enum.hpp"
#include "mapbox/eternal.hpp"

#include "stm32f3xx_hal.h"

class Gpio
{
public:
    /** @brief Zero-overhead, user-friedly and user-safe GPIO API for STM32F303Rx.
     * @example Gpio::writeHigh<Gpio::Pin::A5>() call using arm-none-eabi-gcc (v10.3.1) with -O1 yields 1-3 instructions:
     *  mov.w   r2, #1207959552 ; 0x48000000
     *  movs    r3, #32
     *  str r3, [r2, #24]
     **/

    enum class Pin {
        // clang-format off
        A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15,
        B0, B1, B2, B3, B4, B5, B6, B7, B8, B9, B10, B11, B12, B13, B14, B15,
        C0, C1, C2, C3, C4, C5, C6, C7, C8, C9, C10, C11, C12, C13, C14, C15,
                D2,
        // clang-format on
    };
    enum class PinState { low, high };
    enum class Pull { none, up, down };
    enum class Speed { low, medium, high };
    template <Pin pin> static void initAsInput() { Gpio::hwInit<pin>(Mode::input, Pull::none); }
    template <Pin pin> static void initAsOutput(PinState initial, Pull pull) { hwInitAsOutput<pin>(initial, pull); }
    template <Pin pin>[[nodiscard]] static bool read() noexcept { return hwRead<pin>(); }
    template <Pin pin> static void write(PinState pinState) { hwWrite<pin>(pinState); }
    template <Pin pin> static void writeHigh() { Gpio::hwWrite<pin>(PinState::high); }
    template <Pin pin> static void writeLow() { Gpio::hwWrite<pin>(PinState::low); }
    template <Pin pin> static void toggle() { Gpio::hwToggle<pin>(); }
    template <Pin pin> static void deinit() { Gpio::hwDeinit<pin>(); };
    /** @brief lock pin hardware configuration registers until mcu reboot */
    template <Pin pin> static void lock() { Gpio::hwLock<pin>(); }

private:
    enum class Mode { input, output };

    template <Pin pin> static void hwInitAsOutput(PinState initial, Pull pull) {
        Gpio::hwWrite<pin>(initial);
        Gpio::hwInit<pin>(Mode::output, pull);
    };

    struct PinDescr
    {
        GPIO_TypeDef *hwPort;
        std::uint16_t hwPin;
        void (*preInitFunction)();
    };

    template <Pin pin> static void hwInit(Mode mode, Pull pull) {
        if (getPinDescr<pin>().preInitFunction)
        {
            getPinDescr<pin>().preInitFunction();
        }

        GPIO_InitTypeDef GPIO_InitStruct = {};
        GPIO_InitStruct.Pin = getPinDescr<pin>().hwPin;
        GPIO_InitStruct.Mode = getHwMode(mode);
        GPIO_InitStruct.Pull = getHwPull(pull);
        GPIO_InitStruct.Speed = getHwSpeed(Speed::low);
        HAL_GPIO_Init(getPinDescr<pin>().hwPort, &GPIO_InitStruct);
    }

    template <Pin pin>[[nodiscard]] static bool hwRead() {
        return (getPinDescr<pin>().hwPort->IDR & getPinDescr<pin>().hwPin);
    };

    template <Pin pin> static void hwWrite(PinState pinState) {
        if (pinState == PinState::high)
        {
            getPinDescr<pin>().hwPort->BSRR = static_cast<uint32_t>(getPinDescr<pin>().hwPin);
        }
        else
        { getPinDescr<pin>().hwPort->BRR = static_cast<uint32_t>(getPinDescr<pin>().hwPin); }
    };

    template <Pin pin> static void hwToggle() {
        getPinDescr<pin>().hwPort->BSRR = ((getPinDescr<pin>().hwPort->ODR & getPinDescr<pin>().hwPin) << 16) |
                                          (~getPinDescr<pin>().hwPort->ODR & getPinDescr<pin>().hwPin);
    }

    template <Pin pin> static void hwDeinit() {
        /** @attention related GPIO clock remains enabled. */
        HAL_GPIO_DeInit(getPinDescr<pin>().hwPort, getPinDescr<pin>().hwPin);
    };

    template <Pin pin> static void hwLock() { HAL_GPIO_LockPin(getPinDescr<pin>().hwPort, getPinDescr<pin>().hwPin); }

    static constexpr uint32_t getHwMode(Mode value) {
        constexpr auto map = mapbox::eternal::map<Mode, uint32_t>({
            {Mode::input, GPIO_MODE_INPUT},      //
            {Mode::output, GPIO_MODE_OUTPUT_PP}, //
        });
        return map.find(value)->second;
    }

    static constexpr uint32_t getHwPull(Pull value) {
        constexpr auto map = mapbox::eternal::map<Pull, uint32_t>({
            {Pull::none, GPIO_NOPULL},   //
            {Pull::up, GPIO_PULLUP},     //
            {Pull::down, GPIO_PULLDOWN}, //
        });
        return map.find(value)->second;
    }

    static constexpr uint32_t getHwSpeed(Speed value) {
        constexpr auto map = mapbox::eternal::map<Speed, uint32_t>({
            {Speed::low, GPIO_SPEED_FREQ_LOW},       //
            {Speed::medium, GPIO_SPEED_FREQ_MEDIUM}, //
            {Speed::high, GPIO_SPEED_FREQ_HIGH},     //
        });
        return map.find(value)->second;
    }

    static constexpr GPIO_PinState getHwPinState(PinState value) {
        constexpr auto map = mapbox::eternal::map<PinState, GPIO_PinState>({
            {PinState::low, GPIO_PIN_RESET}, //
            {PinState::high, GPIO_PIN_SET},  //
        });
        return map.find(value)->second;
    }

    static void preInitPortA() { __HAL_RCC_GPIOA_CLK_ENABLE(); }
    static void preInitPortB() { __HAL_RCC_GPIOB_CLK_ENABLE(); }
    static void preInitPortC() { __HAL_RCC_GPIOC_CLK_ENABLE(); }
    static void preInitPortD() { __HAL_RCC_GPIOD_CLK_ENABLE(); }

    template <Pin value> static constexpr PinDescr getPinDescr() {
        constexpr auto map = mapbox::eternal::map<Pin, PinDescr>({
            {Pin::A0, {GPIOA, GPIO_PIN_0, preInitPortA}},   //
            {Pin::A1, {GPIOA, GPIO_PIN_1, preInitPortA}},   //
            {Pin::A2, {GPIOA, GPIO_PIN_2, preInitPortA}},   //
            {Pin::A3, {GPIOA, GPIO_PIN_3, preInitPortA}},   //
            {Pin::A4, {GPIOA, GPIO_PIN_4, preInitPortA}},   //
            {Pin::A5, {GPIOA, GPIO_PIN_5, preInitPortA}},   //
            {Pin::A6, {GPIOA, GPIO_PIN_6, preInitPortA}},   //
            {Pin::A7, {GPIOA, GPIO_PIN_7, preInitPortA}},   //
            {Pin::A8, {GPIOA, GPIO_PIN_8, preInitPortA}},   //
            {Pin::A9, {GPIOA, GPIO_PIN_9, preInitPortA}},   //
            {Pin::A10, {GPIOA, GPIO_PIN_10, preInitPortA}}, //
            {Pin::A11, {GPIOA, GPIO_PIN_11, preInitPortA}}, //
            {Pin::A12, {GPIOA, GPIO_PIN_12, preInitPortA}}, //
            {Pin::A13, {GPIOA, GPIO_PIN_13, preInitPortA}}, //
            {Pin::A14, {GPIOA, GPIO_PIN_14, preInitPortA}}, //
            {Pin::A15, {GPIOA, GPIO_PIN_15, preInitPortA}}, //
            {Pin::B0, {GPIOB, GPIO_PIN_0, preInitPortB}},   //
            {Pin::B1, {GPIOB, GPIO_PIN_1, preInitPortB}},   //
            {Pin::B2, {GPIOB, GPIO_PIN_2, preInitPortB}},   //
            {Pin::B3, {GPIOB, GPIO_PIN_3, preInitPortB}},   //
            {Pin::B4, {GPIOB, GPIO_PIN_4, preInitPortB}},   //
            {Pin::B5, {GPIOB, GPIO_PIN_5, preInitPortB}},   //
            {Pin::B6, {GPIOB, GPIO_PIN_6, preInitPortB}},   //
            {Pin::B7, {GPIOB, GPIO_PIN_7, preInitPortB}},   //
            {Pin::B8, {GPIOB, GPIO_PIN_8, preInitPortB}},   //
            {Pin::B9, {GPIOB, GPIO_PIN_9, preInitPortB}},   //
            {Pin::B10, {GPIOB, GPIO_PIN_10, preInitPortB}}, //
            {Pin::B11, {GPIOB, GPIO_PIN_11, preInitPortB}}, //
            {Pin::B12, {GPIOB, GPIO_PIN_12, preInitPortB}}, //
            {Pin::B13, {GPIOB, GPIO_PIN_13, preInitPortB}}, //
            {Pin::B14, {GPIOB, GPIO_PIN_14, preInitPortB}}, //
            {Pin::B15, {GPIOB, GPIO_PIN_15, preInitPortB}}, //
            {Pin::C0, {GPIOC, GPIO_PIN_0, preInitPortC}},   //
            {Pin::C1, {GPIOC, GPIO_PIN_1, preInitPortC}},   //
            {Pin::C2, {GPIOC, GPIO_PIN_2, preInitPortC}},   //
            {Pin::C3, {GPIOC, GPIO_PIN_3, preInitPortC}},   //
            {Pin::C4, {GPIOC, GPIO_PIN_4, preInitPortC}},   //
            {Pin::C5, {GPIOC, GPIO_PIN_5, preInitPortC}},   //
            {Pin::C6, {GPIOC, GPIO_PIN_6, preInitPortC}},   //
            {Pin::C7, {GPIOC, GPIO_PIN_7, preInitPortC}},   //
            {Pin::C8, {GPIOC, GPIO_PIN_8, preInitPortC}},   //
            {Pin::C9, {GPIOC, GPIO_PIN_9, preInitPortC}},   //
            {Pin::C10, {GPIOC, GPIO_PIN_10, preInitPortC}}, //
            {Pin::C11, {GPIOC, GPIO_PIN_11, preInitPortC}}, //
            {Pin::C12, {GPIOC, GPIO_PIN_12, preInitPortC}}, //
            {Pin::C13, {GPIOC, GPIO_PIN_13, preInitPortC}}, //
            {Pin::C14, {GPIOC, GPIO_PIN_14, preInitPortC}}, // OSC32_IN
            {Pin::C15, {GPIOC, GPIO_PIN_15, preInitPortC}}, // OSC32_OUT
            {Pin::D2, {GPIOD, GPIO_PIN_2, preInitPortD}},   //
        });
        return map.find(value)->second;
    }
};
