#include "system.h"

#include "stm32f3xx_it.h"
#include "stm32f3xx_hal.h"

#include "FreeRTOS.h"
#include "task.h"

#include <stdio.h>

extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim6;

// This function handles Non maskable interrupt.
void NMI_Handler(void)
{
}

#define HALT_IF_DEBUGGING()                                \
    do                                                     \
    {                                                      \
        if ((*(volatile uint32_t *)0xE000EDF0) & (1 << 0)) \
        {                                                  \
            __asm("bkpt 1");                               \
        }                                                  \
    } while (0)

// Disable optimizations for this function so "frame" argument
// does not get optimized away
__attribute__((optimize("O0"))) void hardFaultStackFrameDump(ExceptionStackFrame *exceptionStackFrame, char stackPointerInitial)
{
    // HALT_IF_DEBUGGING();
    FIBSYS_HARDFAULT(exceptionStackFrame, stackPointerInitial);
}

#define HARDFAULT_HANDLING_ASM(_x) \
    __asm volatile(                \
        "tst lr, #4 \n"            \
        "ittee eq \n"              \
        "mrseq r0, msp \n"         \
        "moveq r1, #77 \n"         \
        "mrsne r0, psp \n"         \
        "movne r1, #80 \n"         \
        "b hardFaultStackFrameDump \n")

// This function handles Hard fault interrupt.
void HardFault_Handler(void)
{
    HARDFAULT_HANDLING_ASM();
    while (1)
    {
    }
}

// This function handles Memory management fault.
void MemManage_Handler(void)
{
    FIBSYS_PANIC();
    while (1)
    {
    }
}

// This function handles Pre-fetch fault, memory access fault.
void BusFault_Handler(void)
{
    FIBSYS_PANIC();
    while (1)
    {
    }
}

// This function handles Undefined instruction or illegal state.
void UsageFault_Handler(void)
{
    FIBSYS_PANIC();
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

void TIM6_DAC_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&htim6);
}