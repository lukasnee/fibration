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

    bool txCircular(const std::uint16_t *pData16, std::uint16_t size16, TxIsrCallbacks *pTxIsrCallbacks = nullptr)
    {
        bool retval = false;

        this->txBinarySemaphore.Take();
        if (false == txCircularUnsafe(pData16, size16))
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
    }

    void txHalfCpltIsrCalback()
    {
        if (this->pTxIsrCallbacks)
        {
            this->pTxIsrCallbacks->onTxHalfCompleteIsrCallback();
        }
    }

    bool txCircularStop()
    {
        bool retval = false;

        if (txCircularStopUnsafe())
        {
            retval = this->txBinarySemaphore.Give();
        }

        return retval;
    }

    // TODO: receive packets !

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
    virtual bool txCircularUnsafe(const std::uint16_t *pData16, std::uint16_t size16) = 0;
    virtual bool txCircularStopUnsafe() = 0;

private:
    cpp_freertos::BinarySemaphore txBinarySemaphore;
    TxIsrCallbacks *pTxIsrCallbacks;
};