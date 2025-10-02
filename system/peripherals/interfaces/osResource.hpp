/*
 * Operating System resource locking mechanism
 *
 * Copyright (C) 2021 Lukas Neverauskis <lukas.neverauskis@gmail.com>
 *
 */

#pragma once

#include "FreeRTOS/Semaphore.hpp"

struct OsResource {
public:
    enum Context {
        undefined,
        isr,
        task
    };

    static bool isInIsr(Context context = Context::undefined) {
        return (context == Context::isr)              ? true
               : (context == Context::task)           ? false
               : (xPortIsInsideInterrupt() == pdTRUE) ? true
                                                      : false;
    }

    OsResource() { this->sem.give(); }

    bool lock(TickType_t timeout = portMAX_DELAY, Context context = Context::undefined) {
        bool higherPriorityTaskWoken = false;
        this->isInIsr(context) ? this->sem.takeFromISR(higherPriorityTaskWoken) : this->sem.take(timeout);
        return higherPriorityTaskWoken;
    }

    bool unlock(Context context = Context::undefined) {
        bool higherPriorityTaskWoken = false;
        this->isInIsr(context) ? this->sem.giveFromISR(higherPriorityTaskWoken) : this->sem.give();
        return higherPriorityTaskWoken;
    }

    virtual ~OsResource() = default;

private:
    /** @note Mutexes cannot be used in ISR context. For more info @see
     * https://www.freertos.org/Documentation/02-Kernel/02-Kernel-features/02-Queues-mutexes-and-semaphores/04-Mutexes
     * */
    FreeRTOS::BinarySemaphore sem;
};