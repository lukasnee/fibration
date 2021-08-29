#include "txStream.hpp"

#include "FreeRTOS.h"
#include "thread.hpp"

#include <cstring>

TxStream::TxStream(IODataIF &ioData) : ioData(ioData) {}

bool TxStream::init()
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

bool TxStream::deinit()
{
    bool result = false;

    result = true; // TODO

    return result;
}

void TxStream::reset()
{
    this->headIdx = 0;
    this->tailIdx = 0;
    this->isRolledOver = false;
    this->activeTxSize = 0;
    this->taskHandleToNotify = nullptr;
}

bool TxStream::isEmpty()
{
    return (!isRolledOver && this->tailIdx == this->headIdx);
}

std::size_t TxStream::unbrokenSizeLeft()
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

bool TxStream::in(const std::uint8_t *pData, std::uint32_t size, TickType_t timeout)
{
    return this->in(pData, size, timeout, false);
}
bool TxStream::inFromIsr(const std::uint8_t *pData, std::uint32_t size)
{
    return this->in(pData, size, 0, true);
}

bool TxStream::in(const std::uint8_t *pData, std::uint32_t size, TickType_t timeout, bool isFromIsr)
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

        this->out(isFromIsr);

        if (inSizeLeft)
        {
            /* traffic jam ! */
            if (isFromIsr)
            {
                /* can not wait - ignore ! */
                result = false;
            }
            else
            {
                /* wait for space in queue ! */
                this->taskHandleToNotify = xTaskGetCurrentTaskHandle();
                ulTaskNotifyTake(1, timeout);
                result = this->in(pInDataHead, inSizeLeft);
            }
        }
    }

    return result;
}

bool TxStream::out(bool isFromIsr)
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
        else if (isFromIsr)
        {
            if (false == this->ioData.txFromIsr(&this->buffer[this->tailIdx], this->activeTxSize, this))
            {
                this->activeTxSize = 0;
            }
            else
            {
                result = true;
            }
        }
        else
        {
            if (false == this->ioData.tx(&this->buffer[this->tailIdx], this->activeTxSize, this))
            {
                this->activeTxSize = 0;
            }
            else
            {
                result = true;
            }
        }
    }

    return result;
}

void TxStream::onTxCompleteFromIsr()
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
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken); // there is no task to yield
    }
    this->out(true);
}
