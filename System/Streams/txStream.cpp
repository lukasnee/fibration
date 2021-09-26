/*
 * Data stream out mechanism
 *
 * Copyright (C) 2021 Lukas Neverauskis <lukas.neverauskis@gmail.com>
 *
 */

#include "txStream.hpp"

#include "FreeRTOS.h"
#include "thread.hpp"

#include <cstring>

OutStream::OutStream(IODataIF &ioData) : ioData(ioData) {}

bool OutStream::init()
{
    bool result = false;

    if (this->ioData.init())
    {
        this->reset();
        // this->buffer.fill('\0');
        result = true; // TODO
    }

    return result;
}

bool OutStream::deinit()
{
    bool result = false;

    result = true; // TODO

    return result;
}

void OutStream::reset()
{
    this->headIdx = 0;
    this->tailIdx = 0;
    this->isRolledOver = false;
    this->activeTxSize = 0;
    this->taskHandleToNotify = nullptr;
}

bool OutStream::isEmpty()
{
    return (!isRolledOver && this->tailIdx == this->headIdx);
}

std::size_t OutStream::unbrokenSizeLeft()
{
    std::size_t unbrokenSizeLeft = 0;
    if (this->isRolledOver)
    {
        unbrokenSizeLeft = this->tailIdx - this->headIdx;
    }
    else
    {
        unbrokenSizeLeft = this->buffer.size() - this->headIdx;
    }

    return unbrokenSizeLeft;
}

bool OutStream::in(const std::uint8_t *pData, std::uint32_t size, OsResource::Context context)
{
    bool result = false;

    if (size <= this->buffer.size())
    {
        if (this->isEmpty())
        {
            /* maximize unbrokenSizeLeft */
            this->reset();
        }

        /* fits at all */
        const std::uint8_t *pInDataHead = pData;
        std::uint32_t inSizeLeft = size;
        std::size_t copySize;
        if (inSizeLeft <= this->unbrokenSizeLeft())
        {
            /* append full unbroken */
            copySize = inSizeLeft;
            std::memcpy(&this->buffer[this->headIdx], pInDataHead, copySize);
            this->headIdx += copySize;
            pInDataHead += copySize;
            inSizeLeft -= copySize;
            result = true;
        }
        else if (!this->isRolledOver)
        {
            /* unbroken part */
            std::size_t copySize = this->unbrokenSizeLeft();
            std::memcpy(&this->buffer[this->headIdx], pInDataHead, copySize);
            this->headIdx = 0;
            pInDataHead += copySize;
            inSizeLeft -= copySize;
            this->isRolledOver = true;

            /* broken part */
            copySize = inSizeLeft > this->unbrokenSizeLeft()
                           ? this->unbrokenSizeLeft()
                           : inSizeLeft;
            std::memcpy(&this->buffer[this->headIdx], pInDataHead, copySize);
            this->headIdx += copySize;
            pInDataHead += copySize;
            inSizeLeft -= copySize;
        }

        this->out(context);

        if (inSizeLeft)
        {
            /* traffic jam ! */
            if (context == OsResource::Context::isr)
            {
                /* can not wait - ignore ! */
                result = false;
            }
            else
            {
                /* wait for space in queue ! */
                this->taskHandleToNotify = xTaskGetCurrentTaskHandle();
                ulTaskNotifyTake(1, portMAX_DELAY);
                result = this->in(pInDataHead, inSizeLeft, context);
            }
        }
    }

    return result;
}

bool OutStream::out(OsResource::Context context)
{
    bool result = false;

    if (this->activeTxSize)
    {
        /* ioData tx busy */
    }
    else
    {
        this->activeTxSize = (this->isRolledOver || this->tailIdx > this->headIdx) ? this->buffer.size() - this->tailIdx : this->headIdx - this->tailIdx;

        if (!this->activeTxSize)
        {
            /* queue empty - nothing to do */
            result = true;
        }
        else if (false == this->ioData.txDma(&this->buffer[this->tailIdx], this->activeTxSize, this, context))
        {
            this->activeTxSize = 0;
        }
        else
        {
            result = true;
        }
    }

    return result;
}

void OutStream::onTxCompleteFromIsr()
{
    this->tailIdx += this->activeTxSize;
    this->activeTxSize = 0;
    if (this->tailIdx == this->buffer.size())
    {
        this->isRolledOver = false;
        this->tailIdx = 0;
    }

    if (this->taskHandleToNotify)
    {
        BaseType_t xHigherPriorityTaskWoken = false;
        vTaskNotifyGiveFromISR(this->taskHandleToNotify, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
    this->out(OsResource::Context::isr);
}
