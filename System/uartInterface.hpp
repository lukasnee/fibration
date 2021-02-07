#pragma once

/*
    defines the required interface for uart implementation 
*/

#include <cstdint>


class UartInterface
{
public:
    class Callbacks
    {
    public:
        virtual void txCpltISR() = 0;
        virtual void rxCpltISR() = 0;
    };
    // required interface - copy and implement

    virtual bool init() = 0;
    virtual bool deinit() = 0;
    virtual bool tx(std::uint8_t *pData, std::uint16_t size) = 0;
    virtual bool rx(std::uint8_t *pData, std::uint16_t size) = 0;

    // callbacks handling
    void setCallbacks(Callbacks *pCallbacks = nullptr)
    {
        this->pCallbacks = pCallbacks;
    }

    void txCpltCallback()
    {
        if (this->pCallbacks)
        {
            this->pCallbacks->txCpltISR();
        }
    }

    void rxCpltCallback()
    {
        if (this->pCallbacks)
        {
            this->pCallbacks->rxCpltISR();
        }
    }

private:
    Callbacks *pCallbacks = nullptr;
};