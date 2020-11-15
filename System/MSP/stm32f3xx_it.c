#include "stm32f3xx_hal.h"
#include "stm32f3xx_it.h"

extern TIM_HandleTypeDef htim7;

// This function handles Non maskable interrupt.
void NMI_Handler(void)
{
}

// This function handles Hard fault interrupt.
void HardFault_Handler(void)
{
	while (1)
	{
	}
}

// This function handles Memory management fault.
void MemManage_Handler(void)
{
	while (1)
	{
	}
}

// This function handles Pre-fetch fault, memory access fault.
void BusFault_Handler(void)
{
	while (1)
	{
	}
}

// This function handles Undefined instruction or illegal state.
void UsageFault_Handler(void)
{
	while (1)
	{
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