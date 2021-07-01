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
        virtual void onTxCompleteIsrCallback(){};
    };
    struct RxIsrCallbacks
    {
        virtual void onRxCompleteIsrCallback(){};
    };

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

    bool init()
    {
        bool result = false;

        if (this->isInitialized)
        {
            result = true;
        }
        else
        {
            result = this->initUnsafe();

            if (result)
            {
                this->isInitialized = true;
            }
        }

        return result;
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

    void txCpltIsrCalback()
    {
        if (this->pTxIsrCallbacks)
        {
            this->pTxIsrCallbacks->onTxCompleteIsrCallback();
        }
        BaseType_t xHigherPriorityTaskWoken;
        this->txBinarySemaphore.GiveFromISR(&xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }

    void rxCpltIsrCalback()
    {
        if (this->pRxIsrCallbacks)
        {
            this->pRxIsrCallbacks->onRxCompleteIsrCallback();
        }
        BaseType_t xHigherPriorityTaskWoken;
        this->rxBinarySemaphore.GiveFromISR(&xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
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