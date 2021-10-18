/*
 * Thread-safe full-duplex input/output data API for hardware peripheral such as UART.
 *
 * Copyright (C) 2021 Lukas Neverauskis <lukas.neverauskis@gmail.com>
 *
 */

#pragma once

#include "osResource.hpp"

#include <cstdint>

class IODataIF
{
public:
    IODataIF(){};
    ~IODataIF(){};

    struct IsrTxCallbacks
    {
        virtual void onTxCompleteFromIsr(){};
    };
    struct IsrRxCallbacks
    {
        virtual void onRxCompleteFromIsr(){};
    };

    bool init(OsResource::Context context = OsResource::Context::undefined)
    {
        bool result = false;

        this->osResources.lock(portMAX_DELAY, context);

        if (this->isInitialized)
        {
            result = true;
        }
        else if (this->initUnsafe())
        {
            result = this->isInitialized = true;
        }

        this->osResources.unlock(context);

        return result;
    }

    bool tx(const std::uint8_t *pData, std::size_t size, std::size_t timeout, OsResource::Context context = OsResource::Context::undefined)
    {
        bool retval = false;

        this->osResources.tx.lock(portMAX_DELAY, context);

        retval = this->txUnsafe(pData, size, timeout);

        this->osResources.tx.unlock(context);

        return retval;
    }

    bool txDma(const std::uint8_t *pData, std::size_t size, IsrTxCallbacks *pIsrTxCallbacks = nullptr, OsResource::Context context = OsResource::Context::undefined)
    {
        bool retval = false;

        this->osResources.tx.lock(portMAX_DELAY, context);

        this->pIsrTxCallbacks = pIsrTxCallbacks;
        if (false == this->txDmaUnsafe(pData, size))
        {
            this->pIsrTxCallbacks = nullptr;
            this->osResources.tx.unlock(context);
        }
        else
        {
            retval = true;
        }

        return retval;
    }

    void txDmaCpltIsrCallback()
    {
        BaseType_t xHigherPriorityTaskWoken = this->osResources.tx.unlock(OsResource::Context::isr);

        if (this->pIsrTxCallbacks)
        {
            this->pIsrTxCallbacks->onTxCompleteFromIsr();
        }

        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }

    bool rx(std::uint8_t *pData, std::size_t size, std::size_t timeout, OsResource::Context context = OsResource::Context::undefined)
    {
        bool retval = false;

        this->osResources.rx.lock(portMAX_DELAY, context);

        retval = this->rxUnsafe(pData, size, timeout);

        this->osResources.rx.unlock(context);

        return retval;
    }

    bool rxDma(std::uint8_t *pData, std::size_t size, IsrRxCallbacks *pIsrRxCallbacks = nullptr, OsResource::Context context = OsResource::Context::undefined)
    {
        bool retval = false;

        this->osResources.rx.lock(portMAX_DELAY, context);

        this->pIsrRxCallbacks = pIsrRxCallbacks;
        if (false == this->rxDmaUnsafe(pData, size))
        {
            this->pIsrRxCallbacks = nullptr;
            this->osResources.rx.unlock(context);
        }
        else
        {
            retval = true;
        }

        return retval;
    }

    void rxDmaCpltIsrCallback()
    {
        BaseType_t xHigherPriorityTaskWoken = this->osResources.rx.unlock(OsResource::Context::isr);

        if (this->pIsrRxCallbacks)
        {
            this->pIsrRxCallbacks->onRxCompleteFromIsr();
        }

        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }

    bool deinit(OsResource::Context context = OsResource::Context::undefined)
    {
        bool result = false;

        this->osResources.lock(portMAX_DELAY, context);

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

        this->osResources.unlock(context);

        return result;
    }

protected:
    virtual bool initUnsafe() = 0;
    virtual bool txUnsafe(const std::uint8_t *pData, std::size_t size, std::size_t timeout) = 0;
    virtual bool txDmaUnsafe(const std::uint8_t *pData, std::size_t size) = 0;
    virtual bool rxUnsafe(std::uint8_t *pData, std::size_t size, std::size_t timeout) = 0;
    virtual bool rxDmaUnsafe(std::uint8_t *pData, std::size_t size) = 0;
    virtual bool deinitUnsafe() = 0;

private:
    IsrRxCallbacks *pIsrRxCallbacks = nullptr;
    IsrTxCallbacks *pIsrTxCallbacks = nullptr;

    struct OsResources
    {
        BaseType_t lock(TickType_t timeout = portMAX_DELAY, OsResource::Context context = OsResource::Context::undefined)
        {
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            xHigherPriorityTaskWoken |= this->tx.lock(timeout, context);
            xHigherPriorityTaskWoken |= this->rx.lock(timeout, context);
            return xHigherPriorityTaskWoken;
        }

        BaseType_t unlock(OsResource::Context context = OsResource::Context::undefined)
        {
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            xHigherPriorityTaskWoken |= this->tx.unlock(context);
            xHigherPriorityTaskWoken |= this->rx.unlock(context);
            return xHigherPriorityTaskWoken;
        }

        OsResource tx, rx;

    } osResources;

    bool isInitialized = false;
};