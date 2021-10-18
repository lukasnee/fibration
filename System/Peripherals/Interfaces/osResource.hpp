/*
 * Operating System resource locking mechanism
 *
 * Copyright (C) 2021 Lukas Neverauskis <lukas.neverauskis@gmail.com>
 *
 */

#pragma once

#include <semaphore.hpp>

struct OsResource
{
public:
    enum Context
    {
        undefined,
        isr,
        task
    };

    static bool isInIsr(Context context = Context::undefined)
    {
        return (context == Context::isr) ? true : (context == Context::task)         ? false
                                              : (xPortIsInsideInterrupt() == pdTRUE) ? true
                                                                                     : false;
    }

    OsResource() : binarySemaphore(true){};

    BaseType_t lock(TickType_t timeout = portMAX_DELAY, Context context = Context::undefined)
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        this->isInIsr(context) ? this->binarySemaphore.TakeFromISR(&xHigherPriorityTaskWoken) : this->binarySemaphore.Take(timeout);
        return xHigherPriorityTaskWoken;
    }

    BaseType_t unlock(Context context = Context::undefined)
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        this->isInIsr(context) ? this->binarySemaphore.GiveFromISR(&xHigherPriorityTaskWoken) : this->binarySemaphore.Give();
        return xHigherPriorityTaskWoken;
    }

    virtual ~OsResource() = default;

private:
    cpp_freertos::BinarySemaphore binarySemaphore;
};