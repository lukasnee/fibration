#include "system.hpp"

#include <cstdint>
#include "stm32f3xx_hal.h"
#include "stm32f3xx_it.h"

#include "FreeRTOS.h"
#include "task.h"

int main()
{
    FibSys::run(); // never returns
}

#define STDALLOC true

void * operator new(size_t size)
{
    if(STDALLOC) return std::malloc(size);
    else         return pvPortMalloc(size);
}

void * operator new[](size_t size)
{
    if(STDALLOC) return std::malloc(size);
    else         return pvPortMalloc( size );
}

void operator delete(void * ptr)
{
    if(STDALLOC) std::free(ptr);
    else         vPortFree(ptr);
}

void operator delete[](void * ptr)
{
    if(STDALLOC) std::free(ptr);
    else         vPortFree(ptr);
}

static void Error_Handler()
{
    HardFault_Handler();
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM7)
    {
        HAL_IncTick();
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

    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        Error_Handler();
    }
}

size_t FibSys::initialFreeHeapSize = 0;
size_t FibSys::freeHeapSize = 0; 

TaskHandle_t FibSys::hSystemTask = NULL;
TaskHandle_t FibSys::hMainTask = NULL;

void FibSys::initPlatform()
{
    HAL_Init();
    SystemClock_Config();
}

uint32_t FibSys::getTick()
{
    return HAL_GetTick();
}

void FibSys::taskDelay(std::uint32_t ms)
{
    vTaskDelay(ms / portTICK_PERIOD_MS);
}

void FibSys::panic()
{
    HardFault_Handler(); // todo something better...
}

void FibSys::initTasks(UBaseType_t priority)
{
    BaseType_t xReturned;
    xReturned = xTaskCreate(FibSys::vSystemTask, "system", 0x200, NULL, priority, &FibSys::hSystemTask);	
    if(xReturned == pdFAIL) 
    {
        FibSys::panic();
    }
    
    xReturned = xTaskCreate(FibSys::vMainTask, "main", 0x200, NULL, priority - 1, &FibSys::hMainTask);		
    if(xReturned == pdFAIL) 
    {
        FibSys::panic();
    }
}

void FibSys::run()
{
    const UBaseType_t cSysPriority = configMAX_PRIORITIES - 1;
    FibSys::initPlatform();
    Log::init();
    FibSys::initTasks(cSysPriority);
    uwTick = 0; // for nicer log
    vTaskStartScheduler();
    while(true); // should never be reached
}

void FibSys::vSystemTask(void * pvParams)
{
    FibSys::initialFreeHeapSize = xPortGetFreeHeapSize();
    //Log::system("vSystemTask", "initialFreeHeapSize %lu", freeHeapSize);

    while(true)
    {
        FibSys::freeHeapSize = xPortGetFreeHeapSize();
        //Log::system("vSystemTask", "freeHeapSize %lu", freeHeapSize);
        FibSys::taskDelay(100);
    }
    vTaskDelete(NULL);
}

void FibSys::vMainTask(void * pvParams)
{
    mainTask(pvParams); // should never return
    vTaskDelete(nullptr);
};

void FibSys::stop()
{
    vTaskDelete(FibSys::hSystemTask);
    vTaskDelete(FibSys::hMainTask);
}
