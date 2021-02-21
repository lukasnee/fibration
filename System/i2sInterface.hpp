#pragma once

/*
    defines the required interface for I2S implementation 
*/

#include <cstdint>
#include <semaphore.hpp>

class I2sInterface
{
public:
    struct TxIsrCallbacks
    {
        virtual void onTxCompleteIsrCallback(){};
        virtual void onTxHalfCompleteIsrCallback(){};
    };

    bool txCircular(const std::uint16_t *pData16, std::uint16_t size, TxIsrCallbacks *pTxIsrCallbacks = nullptr)
    {
        bool retval = false;

        this->txBinarySemaphore.Take();
        if (false == txCircularUnsafe(pData16, size))
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

    void txCpltIsrCalback()
    {
        if (this->pTxIsrCallbacks)
        {
            this->pTxIsrCallbacks->onTxCompleteIsrCallback();
        }
        // BaseType_t xHigherPriorityTaskWoken;
        // this->txBinarySemaphore.GiveFromISR(&xHigherPriorityTaskWoken);
        // portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }

    void txHalfCpltIsrCalback()
    {
        if (this->pTxIsrCallbacks)
        {
            this->pTxIsrCallbacks->onTxHalfCompleteIsrCallback();
        }
        // BaseType_t xHigherPriorityTaskWoken;
        // this->txBinarySemaphore.GiveFromISR(&xHigherPriorityTaskWoken);
        // portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
    I2sInterface()
    {
        this->txBinarySemaphore.Give();
    };
    
    ~I2sInterface()
    {
        this->txBinarySemaphore.Take();
    };

    virtual bool init() = 0;
    virtual bool deinit() = 0;

protected:
    virtual bool txCircularUnsafe(const std::uint16_t *pData16, std::uint16_t size) = 0;

private:
    cpp_freertos::BinarySemaphore txBinarySemaphore;
    TxIsrCallbacks *pTxIsrCallbacks;
};