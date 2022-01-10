#include "gpio.hpp"
#include "system.hpp"

#include "fibstd/cast.hpp"
#include "magic_enum.hpp"

#include "stm32f3xx_hal.h"

#include <array>

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
    PinDescr(uint16_t pin, GPIO_TypeDef *port, std::function<void()> preInitFunction)
        : pin(pin), port(port), preInitFunction(preInitFunction){};
    uint16_t pin;
    GPIO_TypeDef *port;
    std::function<void()> preInitFunction;
};

static const std::array<PinDescr, magic_enum::enum_count<Pin>()> pinDescrs = {{
    PinDescr(GPIO_PIN_0, GPIOA, preInitPortA),  //
    PinDescr(GPIO_PIN_1, GPIOA, preInitPortA),  //
    PinDescr(GPIO_PIN_2, GPIOA, preInitPortA),  //
    PinDescr(GPIO_PIN_3, GPIOA, preInitPortA),  //
    PinDescr(GPIO_PIN_4, GPIOA, preInitPortA),  //
    PinDescr(GPIO_PIN_5, GPIOA, preInitPortA),  //
    PinDescr(GPIO_PIN_6, GPIOA, preInitPortA),  //
    PinDescr(GPIO_PIN_7, GPIOA, preInitPortA),  //
    PinDescr(GPIO_PIN_8, GPIOA, preInitPortA),  //
    PinDescr(GPIO_PIN_9, GPIOA, preInitPortA),  //
    PinDescr(GPIO_PIN_10, GPIOA, preInitPortA), //
    PinDescr(GPIO_PIN_11, GPIOA, preInitPortA), //
    PinDescr(GPIO_PIN_12, GPIOA, preInitPortA), //
    PinDescr(GPIO_PIN_13, GPIOA, preInitPortA), //
    PinDescr(GPIO_PIN_14, GPIOA, preInitPortA), //
    PinDescr(GPIO_PIN_15, GPIOA, preInitPortA), //
    PinDescr(GPIO_PIN_0, GPIOB, preInitPortB),  //
    PinDescr(GPIO_PIN_1, GPIOB, preInitPortB),  //
    PinDescr(GPIO_PIN_2, GPIOB, preInitPortB),  //
    PinDescr(GPIO_PIN_3, GPIOB, preInitPortB),  //
    PinDescr(GPIO_PIN_4, GPIOB, preInitPortB),  //
    PinDescr(GPIO_PIN_5, GPIOB, preInitPortB),  //
    PinDescr(GPIO_PIN_6, GPIOB, preInitPortB),  //
    PinDescr(GPIO_PIN_7, GPIOB, preInitPortB),  //
    PinDescr(GPIO_PIN_8, GPIOB, preInitPortB),  //
    PinDescr(GPIO_PIN_9, GPIOB, preInitPortB),  //
    PinDescr(GPIO_PIN_10, GPIOB, preInitPortB), //
    PinDescr(GPIO_PIN_11, GPIOB, preInitPortB), //
    PinDescr(GPIO_PIN_12, GPIOB, preInitPortB), //
    PinDescr(GPIO_PIN_13, GPIOB, preInitPortB), //
    PinDescr(GPIO_PIN_14, GPIOB, preInitPortB), //
    PinDescr(GPIO_PIN_15, GPIOB, preInitPortB), //
    PinDescr(GPIO_PIN_0, GPIOC, preInitPortC),  //
    PinDescr(GPIO_PIN_1, GPIOC, preInitPortC),  //
    PinDescr(GPIO_PIN_2, GPIOC, preInitPortC),  //
    PinDescr(GPIO_PIN_3, GPIOC, preInitPortC),  //
    PinDescr(GPIO_PIN_4, GPIOC, preInitPortC),  //
    PinDescr(GPIO_PIN_5, GPIOC, preInitPortC),  //
    PinDescr(GPIO_PIN_6, GPIOC, preInitPortC),  //
    PinDescr(GPIO_PIN_7, GPIOC, preInitPortC),  //
    PinDescr(GPIO_PIN_8, GPIOC, preInitPortC),  //
    PinDescr(GPIO_PIN_9, GPIOC, preInitPortC),  //
    PinDescr(GPIO_PIN_10, GPIOC, preInitPortC), //
    PinDescr(GPIO_PIN_11, GPIOC, preInitPortC), //
    PinDescr(GPIO_PIN_12, GPIOC, preInitPortC), //
    PinDescr(GPIO_PIN_13, GPIOC, preInitPortC), //
    PinDescr(GPIO_PIN_14, GPIOC, preInitPortC), //
    PinDescr(GPIO_PIN_15, GPIOC, preInitPortC), //
    PinDescr(GPIO_PIN_0, GPIOD, preInitPortD),  //
    PinDescr(GPIO_PIN_1, GPIOD, preInitPortD),  //
    PinDescr(GPIO_PIN_2, GPIOD, preInitPortD),  //
    PinDescr(GPIO_PIN_3, GPIOD, preInitPortD),  //
    PinDescr(GPIO_PIN_4, GPIOD, preInitPortD),  //
    PinDescr(GPIO_PIN_5, GPIOD, preInitPortD),  //
    PinDescr(GPIO_PIN_6, GPIOD, preInitPortD),  //
    PinDescr(GPIO_PIN_7, GPIOD, preInitPortD),  //
    PinDescr(GPIO_PIN_8, GPIOD, preInitPortD),  //
    PinDescr(GPIO_PIN_9, GPIOD, preInitPortD),  //
    PinDescr(GPIO_PIN_10, GPIOD, preInitPortD), //
    PinDescr(GPIO_PIN_11, GPIOD, preInitPortD), //
    PinDescr(GPIO_PIN_12, GPIOD, preInitPortD), //
    PinDescr(GPIO_PIN_13, GPIOD, preInitPortD), //
    PinDescr(GPIO_PIN_14, GPIOD, preInitPortD), //
    PinDescr(GPIO_PIN_15, GPIOD, preInitPortD), //
}};

static constexpr uint32_t getMode(Gpio::Mode mode)
{
    const uint32_t cMap[static_cast<std::size_t>(magic_enum::enum_count<Gpio::Mode>()) + 1] = {
        GPIO_MODE_INPUT, GPIO_MODE_OUTPUT_PP, GPIO_MODE_OUTPUT_OD};
    return cMap[static_cast<std::size_t>(mode)];
}

static constexpr uint32_t getPull(Gpio::Pull pull)
{
    const uint32_t cMap[static_cast<std::size_t>(magic_enum::enum_count<Gpio::Pull>()) + 1] = {GPIO_NOPULL, GPIO_PULLUP,
                                                                                               GPIO_PULLDOWN};
    return cMap[static_cast<std::size_t>(pull)];
}

static constexpr GPIO_PinState getPinState(Gpio::PinState pinState)
{
    const GPIO_PinState cMap[static_cast<std::size_t>(magic_enum::enum_count<Gpio::PinState>()) + 1] = {GPIO_PIN_RESET,
                                                                                                        GPIO_PIN_SET};
    return cMap[static_cast<std::size_t>(pinState)];
}

static void hwInit(Pin pin, Gpio::Mode mode, Gpio::Pull pull)
{
    if (pinDescrs[Fib::Std::Cast::toUnderlying(pin)].preInitFunction)
    {
        pinDescrs[Fib::Std::Cast::toUnderlying(pin)].preInitFunction();
    }

    GPIO_InitTypeDef GPIO_InitStruct = {};
    GPIO_InitStruct.Pin = pinDescrs[Fib::Std::Cast::toUnderlying(pin)].pin;
    GPIO_InitStruct.Mode = getMode(mode);
    GPIO_InitStruct.Pull = getPull(pull);
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(pinDescrs[Fib::Std::Cast::toUnderlying(pin)].port, &GPIO_InitStruct);
}

Gpio &Gpio::getInstance()
{
    static Gpio instance;
    return instance;
}

void Gpio::init(Pin pin, Mode mode, PinState pinStateInitial, Pull pull)
{
    this->write(pin, pinStateInitial);
    hwInit(pin, mode, pull);
}

inline bool Gpio::read(Pin pin)
{
    return static_cast<bool>(HAL_GPIO_ReadPin(pinDescrs[Fib::Std::Cast::toUnderlying(pin)].port,
                                              pinDescrs[Fib::Std::Cast::toUnderlying(pin)].pin));
}

inline void Gpio::write(Pin pin, PinState pinState)
{
    HAL_GPIO_WritePin(pinDescrs[Fib::Std::Cast::toUnderlying(pin)].port,
                      pinDescrs[Fib::Std::Cast::toUnderlying(pin)].pin, getPinState(pinState));
}

inline void Gpio::toggle(Pin pin)
{
    HAL_GPIO_TogglePin(pinDescrs[Fib::Std::Cast::toUnderlying(pin)].port,
                       pinDescrs[Fib::Std::Cast::toUnderlying(pin)].pin);
}

void Gpio::lock(Pin pin)
{
    HAL_GPIO_LockPin(pinDescrs[Fib::Std::Cast::toUnderlying(pin)].port,
                     pinDescrs[Fib::Std::Cast::toUnderlying(pin)].pin);
}

void Gpio::deinit(Pin pin)
{
    HAL_GPIO_DeInit(pinDescrs[Fib::Std::Cast::toUnderlying(pin)].port,
                    pinDescrs[Fib::Std::Cast::toUnderlying(pin)].pin);
    // note gpio related clock still remain enabled
}
