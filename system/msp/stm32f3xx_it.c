// Copyright (c) 2026 Lukas Neverauskis <lukas.neverauskis@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only

#include "ln/ln.h"
#include "ln/port/exceptions.h"

#include "stm32f3xx_it.h"
#include "stm32f3xx_hal.h"

void NMI_Handler(void) {}

void HardFault_Handler(void) {
    LN_HARDFAULT_HANDLER();
    while (1) {
    }
}

void MemManage_Handler(void) {
    LN_PANIC();
    while (1) {
    }
}

void BusFault_Handler(void) {
    LN_PANIC();
    while (1) {
    }
}

void UsageFault_Handler(void) {
    LN_PANIC();
    while (1) {
    }
}

void DebugMon_Handler(void) {}

void TIM7_IRQHandler(void) {
    extern TIM_HandleTypeDef htim7;
    HAL_TIM_IRQHandler(&htim7);
}

void TIM6_DAC_IRQHandler(void) {
    extern TIM_HandleTypeDef htim6;
    HAL_TIM_IRQHandler(&htim6);
}
