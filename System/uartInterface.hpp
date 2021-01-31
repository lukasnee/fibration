#pragma once

/*
    defines the required interface for uart implementation 
*/

#include <cstdint>

class UartInterfaceCallbacks
{
public:
    virtual void txCplt() = 0;
    virtual void rxCplt() = 0;
};

class UartInterface
{
public:
    // required interface - copy and implement
    virtual bool init() = 0;
    virtual bool deinit() = 0;
    virtual bool tx(std::uint8_t *pData, std::uint16_t size) = 0;
    virtual bool rx(std::uint8_t *pData, std::uint16_t size) = 0;

    // callbacks handling
    void setCallbacks(UartInterfaceCallbacks *pUartInterfaceCallbacks = nullptr)
    {
        this->pUartInterfaceCallbacks = pUartInterfaceCallbacks;
    }

    void txCpltCallback()
    {
        if (this->pUartInterfaceCallbacks)
        {
            this->pUartInterfaceCallbacks->txCplt();
        }
    }

    void rxCpltCallback()
    {
        if (this->pUartInterfaceCallbacks)
        {
            this->pUartInterfaceCallbacks->rxCplt();
        }
    }

private:
    UartInterfaceCallbacks *pUartInterfaceCallbacks = nullptr;
};