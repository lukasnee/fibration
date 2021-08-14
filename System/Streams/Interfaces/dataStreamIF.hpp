#pragma once

#include "thread.hpp"
#include "queue.hpp"
#include "stream_buffer.h"

#include <array>

class TxStream
{
public:
    struct TxData
    {
        const uint8_t *pData = nullptr;
        std::uint32_t size = 0;
        bool isDynamic = false;
    };

    struct Config
    {
        static constexpr std::size_t streamBufferSize = 16;
    };

    bool push(const std::uint8_t *pData, std::uint32_t size,
              bool isDynamic = false, TickType_t timeout = portMAX_DELAY)
    {
        const TxData txData = {.pData = pData, .size = size, .isDynamic = isDynamic};
        return this->push(txData, timeout);
    }

    bool pushFromIsr(const std::uint8_t *pData, std::uint32_t size,
                     bool isDynamic = false)
    {
        const TxData txData = {.pData = pData, .size = size, .isDynamic = isDynamic};
        return this->pushFromIsr(txData);
    }

protected:
    virtual bool initTxResource() = 0;
    virtual bool tx(const std::uint8_t *pData, std::uint32_t size) = 0;
    virtual bool txFromIsr(const std::uint8_t *pData, std::uint32_t size) = 0;
    virtual void deinitTxResource() = 0;

    TxStream()
    {
    }

    bool init()
    {
        bool result = false;

        if (this->isStarted)
        {
            result = true;
        }
        else if (false == this->initTxResource())
        {
        }
        else
        {
            this->xStreamBuffer = xStreamBufferCreateStatic(this->ucBufferStorage.size(),
                                                            this->xTriggerLevel,
                                                            this->ucBufferStorage.data(),
                                                            &this->xStreamBufferStruct);
            if (this->xStreamBuffer == nullptr)
            {
            }
            else
            {
                result = this->isStarted = true;
            }
        }

        return result;
    }

    bool txDataCompleteFromIsr()
    {
        bool result = false;

        if (this->txData.pData)
        {
            if (txData.isDynamic)
            {
                delete[] txData.pData;
            }

            this->txData.pData = nullptr;
        }

        size_t xBytesSent;
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xBytesSent = xStreamBufferReceiveFromISR(this->xStreamBuffer,
                                                 &this->txData,
                                                 sizeof(this->txData),
                                                 &xHigherPriorityTaskWoken);
        if (xBytesSent != sizeof(this->txData))
        {
            this->txData.pData = nullptr;
            this->txData.size = 0;
        }
        else if (this->txData.pData == nullptr || this->txData.size == 0)
        {
        }
        else if (false == this->txFromIsr(txData.pData, txData.size))
        {
            result = true;
        }

        return result;
    }

    void deinit()
    {
        if (this->isStarted)
        {
            vStreamBufferDelete(this->xStreamBuffer);
            this->xStreamBuffer = nullptr;
            this->deinitTxResource();
            this->isStarted = false;
        }
    }

private:
    bool push(const TxData &txData, TickType_t timeout = portMAX_DELAY)
    {
        bool result = false;

        if (false == this->isStarted)
        {
        }
        else if (this->txData.pData == nullptr || this->txData.size == 0)
        {
            this->txData = txData;
            result = this->tx(txData.pData, txData.size);
        }
        else
        {
            std::size_t xBytesSent = xStreamBufferSend(this->xStreamBuffer, &txData, sizeof(txData), timeout);
            if (xBytesSent == sizeof(txData))
            {
                result = true;
            }
        }

        return result;
    }

    bool pushFromIsr(const TxData &txData)
    {
        bool result = false;

        if (false == this->isStarted)
        {
        }
        if (this->txData.pData == nullptr || this->txData.size == 0)
        {
            this->txData = txData;
            result = this->tx(txData.pData, txData.size);
        }
        else
        {
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            std::size_t xBytesSent = xStreamBufferSendFromISR(this->xStreamBuffer, &txData, sizeof(txData), &xHigherPriorityTaskWoken);
            if (xBytesSent == sizeof(txData))
            {
                result = true;
            }
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }

        return result;
    }

    TxData txData;
    StaticStreamBuffer_t xStreamBufferStruct;
    StreamBufferHandle_t xStreamBuffer;
    const std::size_t xTriggerLevel = 1;
    std::array<std::uint8_t, Config::streamBufferSize * sizeof(TxData)> ucBufferStorage;
    bool isStarted = false;
};

class RxStream
{
public:
    using RxData = std::uint8_t;

    struct Config
    {
        static constexpr std::size_t streamBufferSize = 256;
    };

    bool pull(RxData &rxData, TickType_t timeout = portMAX_DELAY)
    {
        bool result = false;

        std::size_t xReceivedBytes = xStreamBufferReceive(this->xStreamBuffer, &rxData, sizeof(rxData), timeout);
        if (xReceivedBytes > 0)
        {
            result = true;
        }

        return result;
    }

    bool pullFromISR(RxData &rxData, BaseType_t *pxHigherPriorityTaskWoken)
    {
        bool result = false;

        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        std::size_t xReceivedBytes = xStreamBufferReceiveFromISR(this->xStreamBuffer, &rxData, sizeof(rxData), &xHigherPriorityTaskWoken);
        if (xReceivedBytes > 0)
        {
            result = true;
        }
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

        return result;
    }

protected:
    virtual bool initRxResource() = 0;
    virtual bool rx(std::uint8_t *pData, std::uint32_t size) = 0;
    virtual bool rxFromIsr(std::uint8_t *pData, std::uint32_t size) = 0;
    virtual void deinitRxResource() = 0;

    RxStream()
    {
    }

    bool init()
    {
        bool result = false;

        if (this->isStarted)
        {
            result = true;
        }
        else if (false == this->initRxResource())
        {
        }
        else
        {
            this->xStreamBuffer = xStreamBufferCreateStatic(this->ucBufferStorage.size(),
                                                            this->xTriggerLevel,
                                                            this->ucBufferStorage.data(),
                                                            &this->xStreamBufferStruct);
            if (this->xStreamBuffer == nullptr)
            {
            }
            else if (false == this->rx(&this->rxData, sizeof(RxData)))
            {
            }
            else
            {
                result = this->isStarted = true;
            }
        }

        return result;
    }

    bool rxDataCompleteFromIsr()
    {
        bool result = false;

        size_t xBytesSent;
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xBytesSent = xStreamBufferSendFromISR(this->xStreamBuffer,
                                              &this->rxData,
                                              sizeof(this->rxData),
                                              &xHigherPriorityTaskWoken);

        if (xBytesSent != sizeof(this->rxData))
        {
        }
        else if (false == this->rxFromIsr(&this->rxData, sizeof(this->rxData)))
        {
        }
        else
        {
            result = true;
        }

        return result;
    }

    void deinit()
    {
        if (this->isStarted)
        {
            vStreamBufferDelete(this->xStreamBuffer);
            this->xStreamBuffer = nullptr;
            this->deinitRxResource();
            this->isStarted = false;
        }
    }

private:
    RxData rxData;
    StaticStreamBuffer_t xStreamBufferStruct;
    StreamBufferHandle_t xStreamBuffer;
    const std::size_t xTriggerLevel = 1;
    std::array<RxData, Config::streamBufferSize * sizeof(RxData)> ucBufferStorage;
    bool isStarted = false;
};

// TODO: rename to "class IOStream : public OutStream, public InStream"
class DataStreamIF : public TxStream, public RxStream
{
public:
    DataStreamIF()
    {
    }

    bool init()
    {
        bool result = false;
        if (this->TxStream::init() and this->RxStream::init())
        {
            result = true;
        }
        return result;
    }

    void deinit()
    {
        this->TxStream::deinit();
        this->RxStream::deinit();
    }
};