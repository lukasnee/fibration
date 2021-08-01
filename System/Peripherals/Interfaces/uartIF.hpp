#pragma once

/*
    defines the required interface for uart implementation 
*/

#include <cstdint>
#include <semaphore.hpp>

class UartIF
{
public:
    UartIF() : txBinarySemaphore(true), rxBinarySemaphore(true){};
    ~UartIF(){};

    struct TxIsrCallbacks
    {
        virtual void onTxComplete(){};
    };
    struct RxIsrCallbacks
    {
        virtual void onRxComplete(){};
    };

    bool init()
    {
        bool result = false;

        if (this->isInitialized)
        {
            result = true;
        }
        else if (false == this->initUnsafe())
        {
        }
        else
        {
            result = this->isInitialized = true;
        }

        return result;
    }

    bool tx(const std::uint8_t *pData, std::uint16_t size, TxIsrCallbacks *pTxIsrCallbacks = nullptr)
    {
        bool retval = false;

        this->txBinarySemaphore.Take();
        if (false == txUnsafe(pData, size))
        {
            this->txBinarySemaphore.Give();
        }
        else
        {
            this->pTxIsrCallbacks = pTxIsrCallbacks;
            retval = true;
        }

        return retval;
    }

    bool txFromIsr(const std::uint8_t *pData, std::uint16_t size, TxIsrCallbacks *pTxIsrCallbacks = nullptr)
    {
        bool retval = false;

        BaseType_t xHigherPriorityTaskWoken;
        this->txBinarySemaphore.TakeFromISR(&xHigherPriorityTaskWoken);
        if (false == txUnsafe(pData, size))
        {
            this->txBinarySemaphore.GiveFromISR(&xHigherPriorityTaskWoken);
        }
        else
        {
            this->pTxIsrCallbacks = pTxIsrCallbacks;
            retval = true;
        }
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

        return retval;
    }

    void txCpltIsrCalback()
    {
        BaseType_t xHigherPriorityTaskWoken;
        this->txBinarySemaphore.GiveFromISR(&xHigherPriorityTaskWoken);
        if (this->pTxIsrCallbacks)
        {
            this->pTxIsrCallbacks->onTxComplete();
        }
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }

    bool rx(std::uint8_t *pData, std::uint16_t size, RxIsrCallbacks *pRxIsrCallbacks = nullptr)
    {
        bool retval = false;

        this->rxBinarySemaphore.Take();
        if (false == rxUnsafe(pData, size))
        {
            this->rxBinarySemaphore.Give();
        }
        else
        {
            this->pRxIsrCallbacks = pRxIsrCallbacks;
            retval = true;
        }

        return retval;
    }

    bool rxFromIsr(std::uint8_t *pData, std::uint16_t size, RxIsrCallbacks *pRxIsrCallbacks = nullptr)
    {
        bool retval = false;

        BaseType_t xHigherPriorityTaskWoken;
        this->rxBinarySemaphore.TakeFromISR(&xHigherPriorityTaskWoken);
        if (false == rxUnsafe(pData, size))
        {
            this->rxBinarySemaphore.GiveFromISR(&xHigherPriorityTaskWoken);
        }
        else
        {
            this->pRxIsrCallbacks = pRxIsrCallbacks;
            retval = true;
        }
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

        return retval;
    }

    void rxCpltIsrCalback()
    {
        BaseType_t xHigherPriorityTaskWoken;
        this->rxBinarySemaphore.GiveFromISR(&xHigherPriorityTaskWoken);
        if (this->pRxIsrCallbacks)
        {
            this->pRxIsrCallbacks->onRxComplete();
        }
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }

    bool deinit()
    {
        bool result = false;

        if (this->isInitialized)
        {
            result = this->deinitUnsafe();

            if (result)
            {
                this->isInitialized = false;
            }
        }
        else
        {
            result = true;
        }

        return result;
    }

protected:
    virtual bool initUnsafe() = 0;
    virtual bool deinitUnsafe() = 0;

    virtual bool txUnsafe(const std::uint8_t *pData, std::uint16_t size) = 0;
    virtual bool rxUnsafe(std::uint8_t *pData, std::uint16_t size) = 0;

private:
    cpp_freertos::BinarySemaphore txBinarySemaphore;
    cpp_freertos::BinarySemaphore rxBinarySemaphore;
    TxIsrCallbacks *pTxIsrCallbacks;
    RxIsrCallbacks *pRxIsrCallbacks;
    bool isInitialized = false;
};