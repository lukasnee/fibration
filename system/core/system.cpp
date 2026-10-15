// Copyright (c)  2026 Lukas Neverauskis <lukas.neverauskis@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only

#include "system.hpp"
#include "logger.hpp"
#include "StdStream.hpp"

#include "ln/ln.h"
#include "ln/sysinit.hpp"
#include "ln/logger/logger.h"
#include "ln/shell/CLId.hpp"
#include "ln/prof/prof.h"

extern "C"
{
#include "stm32f3xx_hal.h"
}

LOG_MODULE(system, ln::logger::Level::notset);

extern "C" void vApplicationStackOverflowHook(
    [[maybe_unused]] TaskHandle_t xTask, char *pcTaskName) {
    LOG_CRITICAL("Stack overflow in task {}!", pcTaskName);
    while (true) {
    }
}

extern "C" void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM7) {
        HAL_IncTick();
    }
    if (htim->Instance == TIM6) {
        static bool firstIgnored = false;
        if (!firstIgnored) {
            firstIgnored = true;
            return;
        }
        Periph::getTim6().overflowCallback();
    }
}

static void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {};

    // NUCLEO-F303RE HSE crystal not present
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        LN_PANIC();
    }
    /* Initializes the CPU, AHB and APB buses clocks */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;

    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        LN_PANIC();
    }

    PeriphClkInit.PeriphClockSelection =
        RCC_PERIPHCLK_I2S | RCC_PERIPHCLK_USART3 | RCC_PERIPHCLK_USART2 |
        RCC_PERIPHCLK_ADC12;
    PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
    PeriphClkInit.Usart3ClockSelection =
        RCC_USART3CLKSOURCE_PCLK1; // TODO not sure what the clk NUMBA
    PeriphClkInit.Adc12ClockSelection = RCC_ADC12PLLCLK_DIV1;
    PeriphClkInit.I2sClockSelection = RCC_I2SCLKSOURCE_SYSCLK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
        LN_PANIC();
    }
}

void low_level_init() {
    HAL_Init();
    SystemClock_Config();
}

void FibSys::launch() {
    low_level_init();
    LN_PROF_START();
    get_instance(); // construct the singleton, create init_items...
    vTaskStartScheduler();
}

bool uart2_init() { return Periph::getUart2Stream().init(); }

bool std_stream_init() { return StdStream::getInstance().init(); }

ln::shell::CLId &get_clid_instance() {
    alignas(size_t) static std::array<uint8_t, 1024> task_stack;
    static ln::Task::Cfg task_cfg{
        .prio = ln::Task::Prio::medium,
        .name = "clid",
        .static_stack = task_stack,
    };
    static ln::shell::CLId instance{FibSys::get_cli_instance(), task_cfg};
    return instance;
}

bool cli_service_init() {
    get_clid_instance().start();
    return true;
}

bool adc2_init() {
    if (!Periph::getAdc2().init()) {
        return false;
    }
    if (!Periph::getAdc2().start()) {
        return false;
    }
    return true;
}

bool i2s_streamer_init() {
    static I2sStreamer::Buffer dma_buffer;
    I2sStreamer::Config config{
        .i2s = &Periph::getI2s2(),
        .i2s_config =
            I2sIF::Config{
                .sample_rate_Hz = 44100,
                .sample_bit_depth = 24,
                .sample_frame_size = sizeof(uint32_t),
            },
        .dma_buffer = &dma_buffer,
    };
    return FibSys::get_i2s2_streamer().init(config);
}

bool app_init(void); // project-specific entry point

#define FIBSYSINIT_UNITS(X)                                                    \
    X(logger, logger::init)                                                    \
    X(uart2, uart2_init)                                                       \
    X(std_stream, std_stream_init)                                             \
    X(cli_service, cli_service_init)                                           \
    X(adc2, adc2_init)                                                         \
    X(i2s_streamer, i2s_streamer_init)                                         \
    X(services_target, nullptr)                                                \
    X(app_main, app_init)

/* X(std_stream, logger) - logger does depend on std_stream but not crucial in
 the beginning, since logger buffers output. std_stream is necessary once the
 buffers is full and needs to flushed.
*/
#define FIBSYSINIT_DEPS(X)                                                     \
    X(uart2, std_stream)                                                       \
    X(uart2, cli_service)                                                      \
    X(std_stream, cli_service)                                                 \
    X(adc2, app_main)                                                          \
    X(i2s_streamer, app_main)                                                  \
    X(i2s_streamer, services_target)                                           \
    X(cli_service, services_target)                                            \
    X(services_target, app_main)

LN_SYSINIT_DECLARE(fibsysinit, FIBSYSINIT_UNITS, FIBSYSINIT_DEPS)

void FibSys::startup() { LN_SYSINIT_INIT(fibsysinit); }

void FibSys::Task::taskFunction() {
    this->owner.startup();
    while (true) {
        Task::delay(std::chrono::milliseconds(1000));
        ln::logger::get_instance().flush_buffer();
    }
}
