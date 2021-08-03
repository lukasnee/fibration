#include "uartStream.hpp"
#include "system.hpp"
#include <array>
#include <cstring>

UartStream::UartStream(UartIF &uart) : uart(uart)
{
}

bool UartStream::initTxResource()
{
    bool result = true;

    if (false == this->isTxInitialized && false == this->isRxInitialized)
    {
        result = this->isTxInitialized = this->isRxInitialized = this->uart.init();
    }

    return result;
}

void UartStream::deinitTxResource()
{
    if (true == this->isTxInitialized)
    {
        this->isTxInitialized = false;
        if (false == this->isTxInitialized && false == this->isRxInitialized)
        {
            this->uart.deinit(); // NOTE: result ignored
        }
    }
}

bool UartStream::initRxResource()
{
    return this->initTxResource();
}

void UartStream::deinitRxResource()
{
    if (true == this->isRxInitialized)
    {
        this->isRxInitialized = false;
        if (false == this->isTxInitialized && false == this->isRxInitialized)
        {
            this->uart.deinit(); // NOTE: result ignored
        }
    }
}

bool UartStream::tx(const std::uint8_t *pData, std::uint32_t size)
{
    return this->uart.tx(pData, size, this);
}

bool UartStream::txFromIsr(const std::uint8_t *pData, std::uint32_t size)
{
    return this->uart.txFromIsr(pData, size, this);
}

bool UartStream::rx(std::uint8_t *pData, std::uint32_t size)
{
    return this->uart.rx(pData, size, this);
}

bool UartStream::rxFromIsr(std::uint8_t *pData, std::uint32_t size)
{
    return this->uart.rxFromIsr(pData, size, this);
}

void UartStream::onTxComplete()
{
    this->txDataCompleteFromIsr();
}

void UartStream::onRxComplete()
{
    this->rxDataCompleteFromIsr();
}