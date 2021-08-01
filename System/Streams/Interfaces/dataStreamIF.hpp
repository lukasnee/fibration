#pragma once

#include "thread.hpp"
#include "queue.hpp"
#include "stream_buffer.h"

class TxStream : public cpp_freertos::Thread
{
public:
    struct TxData
    {
        const uint8_t *pData = nullptr;
        std::uint32_t size = 0;
        bool isDataStatic = false;
    };

    bool push(const std::uint8_t *pData, std::uint32_t size,
              bool isDataStatic = false, TickType_t timeout = portMAX_DELAY)
    {
        const TxData txData = {.pData = pData, .size = size, .isDataStatic = isDataStatic};
        return this->push(txData, timeout);
    }

    bool pushFromISR(const std::uint8_t *pData, std::uint32_t size,
                     bool isDataStatic = false, BaseType_t *pxHigherPriorityTaskWoken = nullptr)
    {
        const TxData txData = {.pData = pData, .size = size, .isDataStatic = isDataStatic};
        return this->pushFromISR(txData, pxHigherPriorityTaskWoken);
    }

protected:
    virtual bool initTxResource() = 0;
    virtual bool tx(const std::uint8_t *pData, std::uint32_t size) = 0;
    virtual bool txFromIsr(const std::uint8_t *pData, std::uint32_t size) = 0;
    virtual void deinitTxResource() = 0;

    TxStream(const char *taskName, uint16_t taskStackDepth, UBaseType_t taskPriority, UBaseType_t queueMaxItems)
        : Thread(taskName, taskStackDepth, taskPriority),
          txQueue(queueMaxItems, sizeof(TxData))
    {
    }

    bool init()
    {
        bool result = false;

        if (this->isStarted)
        {
            result = true;
        }
        else
        {
            result = this->Start();

            if (result)
            {
                this->isStarted = true;
            }
        }

        return result;
    }

    void deinit()
    {
        if (this->isStarted)
        {
            this->Suspend();
            this->isStarted = false;
        }
    }

private:
    bool push(const TxData &txData, TickType_t timeout = portMAX_DELAY)
    {
        return this->txQueue.Enqueue(reinterpret_cast<const void *>(&txData), timeout);
    }

    bool pushFromISR(const TxData &txData, BaseType_t *pxHigherPriorityTaskWoken)
    {
        return this->txQueue.EnqueueFromISR(reinterpret_cast<const void *>(&txData), pxHigherPriorityTaskWoken);
    }

    void Run() override
    {
        if (this->initTxResource())
        {
            TxData txData;
            while (true)
            {
                if (this->txQueue.Dequeue(reinterpret_cast<void *>(&txData)))
                {
                    if (this->tx(txData.pData, txData.size))
                    {
                        if (txData.isDataStatic == false)
                        {
                            delete[] txData.pData;
                        }
                    }
                }
            }
        }
        this->deinitTxResource();
    }

    cpp_freertos::Queue txQueue;
    bool isStarted = false;
};

class RxStream
{
public:
    using RxData = std::uint8_t;

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

    RxStream(UBaseType_t queueMaxItems)
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
        else if (false == this->rx(&this->rxData, sizeof(RxData)))
        {
        }
        else
        {
            this->xStreamBuffer = xStreamBufferCreateStatic(sizeof(this->ucBufferStorage),
                                                            this->xTriggerLevel,
                                                            this->ucBufferStorage,
                                                            &this->xStreamBufferStruct);
            if (this->xStreamBuffer)
            {
                result = this->isStarted = true;
            }
        }

        return result;
    }

    bool rxDataFromISR()
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
        else if (false == this->rxFromIsr(&this->rxData, sizeof(RxData)))
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
            this->deinitRxResource();
            this->isStarted = false;
        }
    }

private:
    RxData rxData;
    StaticStreamBuffer_t xStreamBufferStruct;
    StreamBufferHandle_t xStreamBuffer;
    const std::size_t xTriggerLevel = 1;
    std::uint8_t ucBufferStorage[256];
    bool isStarted = false;
};

class DataStreamIF : public TxStream, public RxStream
{
public:
    DataStreamIF(const char *txTaskName, uint16_t txTaskStackDepth, UBaseType_t txTaskPriority, UBaseType_t txQueueMaxItems,
                 UBaseType_t rxQueueMaxItems)
        : TxStream(txTaskName, txTaskStackDepth, txTaskPriority, txQueueMaxItems),
          RxStream(rxQueueMaxItems)
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