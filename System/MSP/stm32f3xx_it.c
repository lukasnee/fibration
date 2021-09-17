#include "stm32f3xx_hal.h"
#include "stm32f3xx_it.h"

#include "FreeRTOS.h"
#include "task.h"

#include "logger.h"

extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim6;

#define BLINK_TIME_MS 100
#define OFF_TIME_MS 500

static void errorLedInit()
{
    static uint8_t errorLedInitialized = 0;
    if (!errorLedInitialized)
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();
        GPIO_InitTypeDef GPIO_InitStruct = {0};
        GPIO_InitStruct.Pin = GPIO_PIN_5;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET); // led off

        errorLedInitialized = 1;
    }
}

static void blinkErrorLed(uint8_t numberOfTimes)
{
    errorLedInit();
    while (numberOfTimes--)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
        HAL_Delay(BLINK_TIME_MS);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
        HAL_Delay(BLINK_TIME_MS);
    }
    HAL_Delay(OFF_TIME_MS);
};

enum TimesToBlinkOnFault
{
    TIMES_TO_BLINK_ON_HARD_FAULT = 1,
    TIMES_TO_BLINK_ON_MEM_FAULT,
    TIMES_TO_BLINK_ON_BUS_FAULT,
    TIMES_TO_BLINK_ON_USAGE_FAULT,
};

// This function handles Non maskable interrupt.
void NMI_Handler(void)
{
}

// This function handles Hard fault interrupt.
void HardFault_Handler(void)
{
    printf("HardFault_Handler\n");
    return;
    while (1)
    {
        blinkErrorLed(TIMES_TO_BLINK_ON_HARD_FAULT);
    }
}

// This function handles Memory management fault.
void MemManage_Handler(void)
{
    printf("MemManage_Handler\n");
    while (1)
    {
        blinkErrorLed(TIMES_TO_BLINK_ON_MEM_FAULT);
    }
}

// This function handles Pre-fetch fault, memory access fault.
void BusFault_Handler(void)
{
    printf("BusFault_Handler\n");
    while (1)
    {
        blinkErrorLed(TIMES_TO_BLINK_ON_BUS_FAULT);
    }
}

// This function handles Undefined instruction or illegal state.
void UsageFault_Handler(void)
{
    printf("UsageFault_Handler\n");
    while (1)
    {
        blinkErrorLed(TIMES_TO_BLINK_ON_USAGE_FAULT);
    }
}

// This function handles Debug monitor.
void DebugMon_Handler(void)
{
}

void TIM7_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&htim7);
}

void TIM6_DAC_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&htim6);
}