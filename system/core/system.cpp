#include "system.hpp"
#include "ln/build.hpp"
#include "resources.hpp"
#include "StdStream.hpp"

#include "ln/ln.h"
#include "ln/logger/logger.hpp"
#include "FreeRTOS/Addons/Clock.hpp"

extern "C"
{
#include "stm32f3xx_hal.h"
}

#include <cstdint>
#include <cstdio>

LOG_MODULE(system, LOGGER_LEVEL_NOTSET);

extern "C" void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM7) {
        HAL_IncTick();
    }
    if (htim->Instance == TIM6) {
        static bool firstIgnored = false;

        if (firstIgnored == false) {
            firstIgnored = true;
        }
        else {
            Periph::getTim6().overflowCallback();
        }
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
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;

    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        LN_PANIC();
    }

    PeriphClkInit.PeriphClockSelection =
        RCC_PERIPHCLK_I2S | RCC_PERIPHCLK_USART3 | RCC_PERIPHCLK_USART2 | RCC_PERIPHCLK_ADC12;
    PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
    PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1; // TODO not sure what the clk NUMBA
    PeriphClkInit.Adc12ClockSelection = RCC_ADC12PLLCLK_DIV1;
    PeriphClkInit.I2sClockSelection = RCC_I2SCLKSOURCE_SYSCLK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
        LN_PANIC();
    }
}

void initPlatform() {
    HAL_Init();
    SystemClock_Config();
}

void FibSys::boot() {
    initPlatform();
    // init system task
    static FibSys fibSys(0x200, FibSys::Priority::highest);
    // start task scheduler
    vTaskStartScheduler();
}

FibSys::FibSys(std::uint16_t stackDepth, BaseType_t priority) : Task(priority, stackDepth, "FibSys"){};

namespace ln::shell {
Cmd version_cmd{Cmd::Cfg{.name = "version", .short_description = "show firmware version", .fn = [](Cmd::Ctx ctx) {
                             ctx.cli.printf("%s v%s [%s] %s %s\n", ln::build::name, ln::build::version::str,
                                            ln::build::git_hash, ln::build::date, ln::build::time);
                             return Err::ok;
                         }}};
} // namespace ln::shell

class CliSvcTask : public FreeRTOS::Task {
public:
    CliSvcTask(ln::shell::CLI &cli, FibSys::Priority priority = FibSys::Priority::low,
               std::uint16_t stack_depth = 0x400, const char *name = "CliSvc")
        : Task(priority, stack_depth, name), cli(cli) {}

private:
    virtual void taskFunction() override {
        while (true) {
            this->cli.routine();
        }
    }
    ln::shell::CLI &cli;
};

void FibSys::startup() {
    {
        const auto res = Periph::init();
        LOG(res ? LOGGER_LEVEL_INFO : LOGGER_LEVEL_ERROR, "Periph::init() = %u", static_cast<unsigned>(res));
    }
    {
        const auto res = StdStream::getInstance().init();
        LOG(res ? LOGGER_LEVEL_INFO : LOGGER_LEVEL_ERROR, "StdStream::init() = %u", static_cast<unsigned>(res));
    }
    auto logger_config = ln::logger::get_instance().get_config();
    logger_config.eol = "\r\n";
    logger_config.enabled_run_time = true;
    ln::logger::get_instance().set_config(logger_config);

    static CliSvcTask CliSvcTask(getCliInstance());
    LOG_INFO("FibSys: starting up %s v%s [%s] %s %s", ln::build::name, ln::build::version::str, ln::build::git_hash,
             ln::build::date, ln::build::time);

    Periph::getAdc2().init();
    Periph::getAdc2().start();
}

void FibSys::taskFunction() {
    this->startup();
    while (true) {
        this->delay(std::chrono::milliseconds(1000));
        ln::logger::get_instance().flush_buffer();
    }
}
