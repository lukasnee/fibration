// Copyright (c)  2026 Lukas Neverauskis <lukas.neverauskis@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only

#include "system.hpp"
#include "logger.hpp"
#include "init.hpp"
#include "StdStream.hpp"

#include "ln/ln.h"
#include "ln/logger/logger.h"

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
    get_instance(); // construct the singleton, create init_items...
    vTaskStartScheduler();
}

namespace ln::shell {
Cmd version_cmd{Cmd::Cfg{.name = "version",
                         .short_description = "show firmware version",
                         .fn = [](Cmd::Ctx ctx) {
                             ctx.cli.print(
                                 "{} v{} [{}] {} {}\n", ln::build::name,
                                 ln::build::version::str, ln::build::git_hash,
                                 ln::build::date, ln::build::time);
                             return Err::ok;
                         }}};
} // namespace ln::shell

void FibSys::startup() {

    enum class ID {
        logger,
        uart2,
        std_stream,
        cli_service,
        adc2,
        i2s_streamer,
        services_target,
        app_main,
    };

    constexpr std::array init_items = {
        init::Item{ID::logger, "logger", []() { return logger::init(); }},
        init::Item{ID::uart2, "uart2",
                   []() { return Periph::getUart2Stream().init(); }},
        init::Item{ID::std_stream, "std_stream",
                   []() { return StdStream::getInstance().init(); }},
        init::Item{ID::cli_service, "cli_service",
                   []() {
                       get_instance().cli_svc_task.start();
                       return true;
                   }},
        init::Item{ID::adc2, "adc2",
                   []() {
                       return Periph::getAdc2().init() &&
                              Periph::getAdc2().start();
                   }},
        init::Item{ID::i2s_streamer, "i2s_streamer",
                   []() {
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
                   }},
        init::Item{ID::services_target, "services_target"},
        init::Item{ID::app_main, "app_main",
                   []() {
                       bool app_init(void); // application-specific entry point
                       return app_init();
                   }},
    };
    constexpr std::array init_deps = {
        /* logger does depend on std_stream but not crucial in the beginning,
         since logger buffers output. std_stream is necessary once the buffers
         is full and needs to flushed.
        */
        // init::Dep{ID::std_stream, ID::logger},
        init::Dep{ID::uart2, ID::std_stream},
        init::Dep{ID::uart2, ID::cli_service},
        init::Dep{ID::std_stream, ID::cli_service},
        init::Dep{ID::adc2, ID::app_main},
        init::Dep{ID::i2s_streamer, ID::app_main},
        init::Dep{ID::i2s_streamer, ID::services_target},
        init::Dep{ID::cli_service, ID::services_target},
        init::Dep{ID::services_target, ID::app_main},
    };
    static constexpr auto ordered_initializers =
        ln::algo::CompileTimeDAG::Graph(init_items, init_deps).topo_sort();
    init::init(ordered_initializers);
}

void FibSys::Task::taskFunction() {
    this->owner.startup();
    while (true) {
        Task::delay(std::chrono::milliseconds(1000));
        ln::logger::get_instance().flush_buffer();
    }
}
