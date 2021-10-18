/*
 * Thread-safe input/output peripheral data general API
 *
 * Copyright (C) 2021 Lukas Neverauskis <lukas.neverauskis@gmail.com>
 *
 */

#pragma once

#include "osResource.hpp"

#include <cstdint>

class IODataIF : public OsResource
{
public:
    IODataIF() : OsResource(){};
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

        this->lock(portMAX_DELAY, context);

        if (this->isInitialized)
        {
            result = true;
        }
        else
        {
            if (this->initUnsafe())
            {
                result = this->isInitialized = true;
            }
        }

        this->unlock(context);

        return result;
    }

    bool tx(const std::uint8_t *pData, std::size_t size, std::size_t timeout, OsResource::Context context = OsResource::Context::undefined)
    {
        bool retval = false;

        this->lock(portMAX_DELAY, context);

        retval = this->txUnsafe(pData, size, timeout);

        this->unlock(context);

        return retval;
    }

    bool txDma(const std::uint8_t *pData, std::size_t size, IsrTxCallbacks *pIsrTxCallbacks = nullptr, OsResource::Context context = OsResource::Context::undefined)
    {
        bool retval = false;

        this->lock(portMAX_DELAY, context);

        this->pIsrTxCallbacks = pIsrTxCallbacks;
        if (false == this->txDmaUnsafe(pData, size))
        {
            this->pIsrTxCallbacks = nullptr;
            this->unlock(context);
        }
        else
        {
            retval = true;
        }

        return retval;
    }

    void txDmaCpltIsrCallback()
    {
        BaseType_t xHigherPriorityTaskWoken = this->unlock(OsResource::Context::isr);

        if (this->pIsrTxCallbacks)
        {
            this->pIsrTxCallbacks->onTxCompleteFromIsr();
        }

        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }

    bool rx(std::uint8_t *pData, std::size_t size, std::size_t timeout, OsResource::Context context = OsResource::Context::undefined)
    {
        bool retval = false;

        this->lock(portMAX_DELAY, context);

        retval = this->rxUnsafe(pData, size, timeout);

        this->unlock(context);

        return retval;
    }

    bool rxDma(std::uint8_t *pData, std::size_t size, IsrRxCallbacks *pIsrRxCallbacks = nullptr, OsResource::Context context = OsResource::Context::undefined)
    {
        bool retval = false;

        this->lock(portMAX_DELAY, context);

        this->pIsrRxCallbacks = pIsrRxCallbacks;
        if (false == this->rxDmaUnsafe(pData, size))
        {
            this->pIsrRxCallbacks = nullptr;
            this->unlock(context);
        }
        else
        {
            retval = true;
        }

        return retval;
    }

    void rxDmaCpltIsrCallback()
    {
        BaseType_t xHigherPriorityTaskWoken = this->unlock(OsResource::Context::isr);

        if (this->pIsrRxCallbacks)
        {
            this->pIsrRxCallbacks->onRxCompleteFromIsr();
        }

        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }

    bool deinit(OsResource::Context context = OsResource::Context::undefined)
    {
        bool result = false;

        this->lock(portMAX_DELAY, context);

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

        this->unlock(context);

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
    bool isInitialized = false;
};