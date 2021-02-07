#pragma once

/*
    defines the required interface for uart implementation 
*/

#include <cstdint>
#include <semaphore.hpp>

class UartInterface
{
public:
    struct TxIsrCallbacks
    {
        virtual void onTxComplete(){};
    };
    struct RxIsrCallbacks
    {
        virtual void onRxComplete(){};
    };

    bool tx(const std::uint8_t *pData, std::uint16_t size, TxIsrCallbacks *pTxIsrCallbacks = nullptr)
    {
        bool retval = false;

        this->txSemaphore.Take();
        if (false == txUnsafe(pData, size))
        {
            this->txSemaphore.Give();
        }
        else
        {
            this->pTxIsrCallbacks = pTxIsrCallbacks;
            retval = true;
        }

        return retval;
    }

    bool rx(std::uint8_t *pData, std::uint16_t size, RxIsrCallbacks *pRxIsrCallbacks = nullptr)
    {
        bool retval = false;

        this->rxSemaphore.Take();
        if (false == rxUnsafe(pData, size))
        {
            this->rxSemaphore.Give();
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
        BaseType_t pxHigherPriorityTaskWoken;
        this->txSemaphore.GiveFromISR(&pxHigherPriorityTaskWoken);
        if (this->pTxIsrCallbacks)
        {
            this->pTxIsrCallbacks->onTxComplete();
        }
    }

    void rxCpltIsrCalback()
    {
        BaseType_t pxHigherPriorityTaskWoken;
        this->rxSemaphore.GiveFromISR(&pxHigherPriorityTaskWoken);
        if (this->pRxIsrCallbacks)
        {
            this->pRxIsrCallbacks->onRxComplete();
        }
    }

    UartInterface()
    {
        this->txSemaphore.Give();
        this->rxSemaphore.Give();
    };

    virtual bool init() = 0;
    virtual bool deinit() = 0;

protected:
    virtual bool txUnsafe(const std::uint8_t *pData, std::uint16_t size) = 0;
    virtual bool rxUnsafe(std::uint8_t *pData, std::uint16_t size) = 0;

private:
    cpp_freertos::BinarySemaphore txSemaphore;
    cpp_freertos::BinarySemaphore rxSemaphore;
    TxIsrCallbacks *pTxIsrCallbacks;
    RxIsrCallbacks *pRxIsrCallbacks;
};