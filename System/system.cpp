#include "system.hpp"
#include "peripherals.hpp"
#include "log.hpp"
#include "uartStream.hpp"
#include "shell.hpp"
#include "uartStreamer.hpp"

#include "ticks.hpp"
#include "timer.hpp"

#include "stm32f3xx_hal.h"
#include "stm32f3xx_it.h"

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

    /* Initializes the RCC Oscillators according to the specified parameters
    in the RCC_OscInitTypeDef structure. */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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

    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1 | RCC_PERIPHCLK_USART2;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
    PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        FibSys::panic();
    }
}

void sInitPlatform()
{
    HAL_Init();
    SystemClock_Config();
}

uint32_t FibSys::getSysTick()
{
    return HAL_GetTick();
}

void FibSys::panic()
{
    Log::direct("\r\n\nsystem panic!\r\n\n"sv); // TODO does not work?!
    vTaskDelay(cpp_freertos::Ticks::MsToTicks(1000));
    vTaskSuspendAll();
    taskDISABLE_INTERRUPTS();
    while (true)
    {
    };
}

BaseType_t FibSys::getSysMaxPriority()
{
    return configMAX_PRIORITIES - 1;
}

BaseType_t FibSys::getAppMaxPriority()
{
    return getSysMaxPriority() - 1;
}

void FibSys::start()
{
    sInitPlatform();
    // init system task
    static FibSys fibSys(0x200, getSysMaxPriority());
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

#include <utility>
#include <limits>
#include <cmath>

using I2sData = std::pair<std::uint16_t, std::uint16_t>;
using I2sDataBuffer = std::array<I2sData, 0x40>;
using I2sDataBufferCircluar = std::pair<I2sDataBuffer, I2sDataBuffer>;

static I2sDataBufferCircluar I2s2DataBufferCircluar;

//FibSys thread
void FibSys::Run()
{
    static auto uartStreamerForLog = UartStreamer(Periph::getUart1(), "logUartStreamer", 0x200, 10);
    Log::setUartStreamer(&uartStreamerForLog);

    static auto uartStreamForShell = UartStream(Periph::getUart2());
    Shell::start(uartStreamForShell, 0x200, 10);

    //static auto i2s2StreamForAudio = I2s2

    std::uint16_t level16 = 0;
    std::uint16_t u16max = std::numeric_limits<std::uint16_t>::max();

    std::size_t it = 0;
    for(auto &val16 : I2s2DataBufferCircluar.first)
    {
        val16.first = static_cast<std::uint16_t>((std::sin(2* M_PI * it / I2s2DataBufferCircluar.first.size()) / 3.0f + 1.0f) * u16max);
        val16.second = val16.first;
        it++;
        //level16 += u16max/0x400;
        // c.first = level16;
        // c.second = level16 += std::numeric_limits<std::uint16_t>::max()/0x400;
    } 
    level16 = 0;
    it = 0;
    for(auto &val16 : I2s2DataBufferCircluar.second)
    {
        val16.first = static_cast<std::uint16_t>((std::sin(2* M_PI * it / I2s2DataBufferCircluar.first.size()) / 3.0f + 1.0f) * u16max);
        val16.second = val16.first;
        it++;
        // c.first = level16;
        // c.second = level16 -= std::numeric_limits<std::uint16_t>::max()/0x400;
    }

    Periph::getI2s2().txCircular(&I2s2DataBufferCircluar.first.at(0).first, 2 * I2s2DataBufferCircluar.first.size(), nullptr);

    while (true)
    {
        // TODO:
        // this->collectStats();
        Delay(cpp_freertos::Ticks::MsToTicks(1000));
    }
    vTaskDelete(NULL);
}
