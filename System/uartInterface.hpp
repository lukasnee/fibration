#pragma once

/*
    defines the required interface for uart implementation 
*/

#include <cstdint>

class UartInterface
{
public:
    // required interface - copy and implement
    virtual bool init() = 0;
    virtual bool deinit() = 0;
    virtual bool tx(std::uint8_t *pData, std::uint16_t size, void (*txCpltCalbackISR)(void) = nullptr) = 0;
    virtual bool rx(std::uint8_t *pData, std::uint16_t size, void (*rxCpltCalbackISR)(void) = nullptr) = 0;


    // callback handling:
    void txCpltCallback()
    {
        if (this->txCpltCalbackISR)
        {
            this->txCpltCalbackISR();
        }
    }

    void rxCpltCallback()
    {
        if (this->rxCpltCalbackISR)
        {
            this->rxCpltCalbackISR();
        }
    }

private:
    void (*txCpltCalbackISR)(void) = nullptr;
    void (*rxCpltCalbackISR)(void) = nullptr;
};