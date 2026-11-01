// Copyright (c) 2026 Lukas Neverauskis <lukas.neverauskis@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only

#include "ln/ln.h"

#include "uart2.hpp"
#include "uart3.hpp"

#include "stm32f3xx_hal.h"

extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

extern "C" void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    bool result = false;
    bool higher_priority_thread_woken = false;
    if (huart == &huart2) {
        std::tie(result, higher_priority_thread_woken) =
            Uart2::get_instance().ll_async_write_completed_cb_from_isr();
    }
    else if (huart == &huart3) {
        std::tie(result, higher_priority_thread_woken) =
            Uart3::get_instance().ll_async_write_completed_cb_from_isr();
    }
    if (result) {
        ln::yield_from_isr(higher_priority_thread_woken);
    }
}

extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    bool result = false;
    bool higher_priority_thread_woken = false;
    if (huart == &huart2) {
        std::tie(result, higher_priority_thread_woken) =
            Uart2::get_instance().ll_async_read_completed_cb_from_isr();
    }
    else if (huart == &huart3) {
        std::tie(result, higher_priority_thread_woken) =
            Uart3::get_instance().ll_async_read_completed_cb_from_isr();
    }
    if (result) {
        ln::yield_from_isr(higher_priority_thread_woken);
    }
}
