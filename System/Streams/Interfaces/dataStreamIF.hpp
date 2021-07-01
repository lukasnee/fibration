#pragma once

#include "thread.hpp"
#include "queue.hpp"

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

class RxStream : public cpp_freertos::Thread
{
public:
    using RxData = std::uint8_t;

    bool pull(RxData &rxData, TickType_t timeout = portMAX_DELAY)
    {
        return this->rxQueue.Dequeue(reinterpret_cast<void *>(&rxData), timeout);
    }

    bool pullFromISR(RxData &rxData, BaseType_t *pxHigherPriorityTaskWoken)
    {
        return this->rxQueue.DequeueFromISR(reinterpret_cast<void *>(&rxData), pxHigherPriorityTaskWoken);
    }

protected:
    virtual bool initRxResource() = 0;
    virtual bool rx(std::uint8_t *pData, std::uint32_t size) = 0;
    virtual void deinitRxResource() = 0;

    RxStream(const char *taskName, uint16_t taskStackDepth, UBaseType_t taskPriority, UBaseType_t queueMaxItems)
        : Thread(taskName, taskStackDepth, taskPriority), rxQueue(queueMaxItems, sizeof(RxData))
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
    void Run() override
    {
        if (this->initRxResource())
        {
            constexpr TickType_t timeout = portMAX_DELAY;
            RxData rxData;
            while (true)
            {
                if (this->rx(reinterpret_cast<std::uint8_t *>(&rxData), sizeof(rxData)))
                {
                    this->rxQueue.Enqueue(reinterpret_cast<void *>(&rxData), timeout);
                }
            }
        }
        this->deinitRxResource();
    }

    cpp_freertos::Queue rxQueue;
    bool isStarted = false;
};

class DataStreamIF : public TxStream, public RxStream
{
public:
    DataStreamIF(const char *txTaskName, uint16_t txTaskStackDepth, UBaseType_t txTaskPriority, UBaseType_t txQueueMaxItems,
                 const char *rxTaskName, uint16_t rxTaskStackDepth, UBaseType_t rxTaskPriority, UBaseType_t rxQueueMaxItems)
        : TxStream(txTaskName, txTaskStackDepth, txTaskPriority, txQueueMaxItems),
          RxStream(rxTaskName, rxTaskStackDepth, rxTaskPriority, rxQueueMaxItems)
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