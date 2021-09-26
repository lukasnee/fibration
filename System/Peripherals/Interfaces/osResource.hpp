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

protected:
    OsResource() : txBinarySemaphore(true), rxBinarySemaphore(true){};

    BaseType_t lock(TickType_t timeout = portMAX_DELAY, Context context = Context::undefined)
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        this->isInIsr(context) ? this->txBinarySemaphore.TakeFromISR(&xHigherPriorityTaskWoken) : this->txBinarySemaphore.Take(timeout);
        return xHigherPriorityTaskWoken;
    }

    BaseType_t unlock(Context context = Context::undefined)
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        this->isInIsr(context) ? this->txBinarySemaphore.GiveFromISR(&xHigherPriorityTaskWoken) : this->txBinarySemaphore.Give();
        return xHigherPriorityTaskWoken;
    }

    virtual ~OsResource() = default;

private:
    bool isInIsr(Context context = Context::undefined)
    {
        const bool isInIsr = (context == Context::isr) ? true : (context == Context::task)         ? false
                                                            : (xPortIsInsideInterrupt() == pdTRUE) ? true
                                                                                                   : false;
    }

    cpp_freertos::BinarySemaphore txBinarySemaphore;
    cpp_freertos::BinarySemaphore rxBinarySemaphore;
};