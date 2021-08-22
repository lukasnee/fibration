#pragma once

#include <cstdint>
#include <semaphore.hpp>

/**
 * @brief thread-safe generalized input/output data interface.
 */
class IODataIF
{
public:
    IODataIF() : txBinarySemaphore(true), rxBinarySemaphore(true){};
    ~IODataIF(){};

    struct IsrCallbacks
    {
        virtual void onTxCompleteFromIsr(){};
        virtual void onRxCompleteFromIsr(){};
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

    bool tx(const std::uint8_t *pData, std::uint16_t size, IsrCallbacks *pIsrCallbacks = nullptr)
    {
        bool retval = false;

        this->txBinarySemaphore.Take();
        this->pIsrCallbacks = pIsrCallbacks;
        if (false == txUnsafe(pData, size))
        {
            this->txBinarySemaphore.Give();
            this->pIsrCallbacks = nullptr;
        }
        else
        {
            retval = true;
        }

        return retval;
    }

    bool txFromIsr(const std::uint8_t *pData, std::uint16_t size, IsrCallbacks *pIsrCallbacks = nullptr)
    {
        bool retval = false;

        BaseType_t xHigherPriorityTaskWoken;
        this->txBinarySemaphore.TakeFromISR(&xHigherPriorityTaskWoken);
        this->pIsrCallbacks = pIsrCallbacks;
        if (false == txUnsafe(pData, size))
        {
            this->txBinarySemaphore.GiveFromISR(&xHigherPriorityTaskWoken);
            this->pIsrCallbacks = nullptr;
        }
        else
        {
            retval = true;
        }
        // portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

        return retval;
    }

    void txCpltIsrCalback()
    {
        BaseType_t xHigherPriorityTaskWoken;
        this->txBinarySemaphore.GiveFromISR(&xHigherPriorityTaskWoken);
        if (this->pIsrCallbacks)
        {
            this->pIsrCallbacks->onTxCompleteFromIsr();
        }
        // portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }

    bool rx(std::uint8_t *pData, std::uint16_t size, IsrCallbacks *pIsrCallbacks = nullptr)
    {
        bool retval = false;

        this->rxBinarySemaphore.Take();
        this->pIsrCallbacks = pIsrCallbacks;
        if (false == rxUnsafe(pData, size))
        {
            this->rxBinarySemaphore.Give();
            this->pIsrCallbacks = nullptr;
        }
        else
        {
            retval = true;
        }

        return retval;
    }

    bool rxFromIsr(std::uint8_t *pData, std::uint16_t size, IsrCallbacks *pIsrCallbacks = nullptr)
    {
        bool retval = false;

        BaseType_t xHigherPriorityTaskWoken;
        this->rxBinarySemaphore.TakeFromISR(&xHigherPriorityTaskWoken);
        this->pIsrCallbacks = pIsrCallbacks;
        if (false == rxUnsafe(pData, size))
        {
            this->rxBinarySemaphore.GiveFromISR(&xHigherPriorityTaskWoken);
            this->pIsrCallbacks = nullptr;
        }
        else
        {
            retval = true;
        }
        // portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

        return retval;
    }

    void rxCpltIsrCalback()
    {
        BaseType_t xHigherPriorityTaskWoken;
        this->rxBinarySemaphore.GiveFromISR(&xHigherPriorityTaskWoken);
        if (this->pIsrCallbacks)
        {
            this->pIsrCallbacks->onRxCompleteFromIsr();
        }
        // portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
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
    IsrCallbacks *pIsrCallbacks = nullptr;
    bool isInitialized = false;
};