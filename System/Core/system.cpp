#include "system.hpp"
#include "version.hpp"
#include "resources.hpp"
#include "logger.hpp"
#include "ioStream.hpp"
#include "shell.hpp"

#include "ticks.hpp"
#include "timer.hpp"

extern "C"
{
#include "stm32f3xx_hal.h"
#include "stm32f3xx_it.h"
}
#include <limits>
#include <cstdint>

extern "C" void vApplicationMallocFailedHook(void)
{
    FibSys::panic();
}

extern "C" void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM7)
    {
        HAL_IncTick();
    }
    if (htim->Instance == TIM6)
    {
        static bool firstIgnored = false;

        if (firstIgnored == false)
        {
            firstIgnored = true;
        }
        else
        {
            Periph::getTim6().overflowCallback();
        }
    }
}

static void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    // NUCLEO-F303RE HSE crystal not present
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        FibSys::panic();
    }
    /* Initializes the CPU, AHB and APB buses clocks */
    RCC_ClkInitStruct.ClockType =
        RCC_CLOCKTYPE_HCLK |
        RCC_CLOCKTYPE_SYSCLK |
        RCC_CLOCKTYPE_PCLK1 |
        RCC_CLOCKTYPE_PCLK2;

    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        FibSys::panic();
    }

    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2S | RCC_PERIPHCLK_USART3 | RCC_PERIPHCLK_USART2 | RCC_PERIPHCLK_ADC12;
    PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
    PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1; // TODO not sure what the clk NUMBA
    PeriphClkInit.Adc12ClockSelection = RCC_ADC12PLLCLK_DIV1;
    PeriphClkInit.I2sClockSelection = RCC_I2SCLKSOURCE_SYSCLK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        FibSys::panic();
    }
}

void initPlatform()
{
    HAL_Init();
    SystemClock_Config();
}

uint32_t FibSys::getSysTick()
{
    return HAL_GetTick();
}

std::uint32_t FibSys::getUptimeInMs()
{
    return cpp_freertos::Ticks::TicksToMs(FibSys::getSysTick());
}

void FibSys::getUptime(std::uint32_t &days, std::uint32_t &hours, std::uint32_t &minutes, std::uint32_t &seconds, std::uint32_t &milliseconds)
{
    std::uint32_t uptimeInMs = getUptimeInMs();
    std::uint32_t secondsTotal = uptimeInMs / 1000;
    std::uint32_t minutesTotal = secondsTotal / 60;
    std::uint32_t hoursTotal = minutesTotal / 60;
    std::uint32_t daysTotal = hoursTotal / 24;

    milliseconds = uptimeInMs % 1000;
    seconds = secondsTotal % 60;
    minutes = minutesTotal % 60;
    hours = hoursTotal % 24;
    days = daysTotal;
}

void FibSys::panic()
{
    Logger::log("\n\nsystem panic !\n"
                "SysTick: %lu\n\n"sv,
                FibSys::getSysTick()); // TODO does not work?!
    vTaskDelay(cpp_freertos::Ticks::MsToTicks(1000));
    vTaskSuspendAll();
    taskDISABLE_INTERRUPTS();
    while (true)
    {
    };
}

void FibSys::boot()
{
    initPlatform();
    // init system task
    static FibSys fibSys(0x200, FibSys::Priority::highest);
    // start task scheduler
    vTaskStartScheduler();
}

FibSys::FibSys(std::uint16_t stackDepth, BaseType_t priority) : Thread("FibSys", stackDepth, priority)
{
    if (Start() == false)
    {
        FibSys::panic();
    }
};

// void FibSys::collectStats()
// {
//     std::size_t minFreeHeapSize = std::numeric_limits<std::size_t>::max();

//     std::size_t freeHeapSize = xPortGetFreeHeapSize();
//     if(freeHeapSize < minFreeHeapSize)
//     {
//         minFreeHeapSize = freeHeapSize;
//     }
// }

//FibSys thread
void FibSys::Run()
{
    static IOStream ioStreamUart2(Periph::getUart2());
    static AsciiStream textStreamUart2(ioStreamUart2);
    if (false == Logger::setAsciiStream(textStreamUart2))
    {
        FibSys::panic();
    }
    Logger::log(Logger::Verbosity::high, Logger::Type::system, "Fibration %s v%u.%u.%u\n", Fib::Version::moduleName, Fib::Version::major, Fib::Version::minor, Fib::Version::patch);
    Shell::start(textStreamUart2, 0x200, FibSys::Priority::appHigh);

    Periph::getAdc2().init();
    Periph::getAdc2().start();

    while (true)
    {
        // TODO:
        // this->collectStats();

        Delay(cpp_freertos::Ticks::MsToTicks(1000));
    }
}
