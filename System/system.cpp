#include "system.hpp"
#include "peripherals.hpp"
#include "log.hpp"

#include "ticks.hpp"
#include "timer.hpp"

#include "stm32f3xx_hal.h"
#include "stm32f3xx_it.h"

#include <limits>
#include <cstdint>

extern "C" void vApplicationMallocFailedHook( void )
{
    FibSys::panic();
}

static void Error_Handler()
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
        
        if(firstIgnored == false) 
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
    RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
    RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
    RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

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
        Error_Handler();
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
        Error_Handler();
    }

    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART2;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
    PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        Error_Handler();
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
    Log::DIRECT("\r\n\nsystem panic!\r\n\n");
    HardFault_Handler(); // todo something better...
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
    // init hardware
    sInitPlatform();

    Log::init(); // todo cleanup
    
    // init system level tasks
    static FibSys fibSys(0x200, getSysMaxPriority());

    // start task scheduler
    uwTick = 0; // for nicer log
    vTaskStartScheduler();

    while(true); // should never be reached
}

FibSys::FibSys(std::uint16_t stackDepth, BaseType_t priority) : 
    Thread("FibSys", stackDepth, priority) 
{
    if(Start() == false)
    {
        FibSys::panic();
    }
};

extern std::uint32_t callCount;
extern "C" uint32_t uint32GetRunTimeCounterValue();
//FibSys thread
void FibSys::Run() {
    std::size_t minFreeHeapSize = std::numeric_limits<std::size_t>::max();
    std::size_t initialFreeHeapSize = xPortGetFreeHeapSize();



    while(true)
    {
        std::size_t freeHeapSize = xPortGetFreeHeapSize();
        if(freeHeapSize < minFreeHeapSize)
        {
            minFreeHeapSize = freeHeapSize;
        }

        char szRunTimeStats[10*40];
        vTaskGetRunTimeStats(szRunTimeStats);
        
        Log::clear();
        Log::system("FibSys", "\n\r"
            "initialFreeHeapSize:%lu\n\r"
            "       freeHeapSize:%lu\n\r"
            "    minFreeHeapSize:%lu\n\r"
            " RunTimeStatsCntVal:%lu\n\r"
            "Task\t\ttime,.1ms\ttime,%%\n\r%s",
            initialFreeHeapSize,
            freeHeapSize,
            minFreeHeapSize,
            uint32GetRunTimeCounterValue(),
            szRunTimeStats);

        Delay(cpp_freertos::Ticks::MsToTicks(500));
    }
    vTaskDelete(NULL);
}

