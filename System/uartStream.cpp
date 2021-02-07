#include "uartStream.hpp"
#include "system.hpp"
#include <array>

UartStream::UartStream(UartInterface &uart) : Stream(), uart(uart)
{
    this->uart.init();
}

UartStream::~UartStream()
{
    this->uart.deinit();
}

void UartStream::onTxComplete()
{
    if (this->pOwner)
    {
        this->pOwner->ResumeFromISR();
    }
}

void UartStream::onRxComplete()
{
    if (this->pOwner)
    {
        this->pOwner->ResumeFromISR();
    }
}

void UartStream::putc(char c)
{
    this->uart.tx(reinterpret_cast<std::uint8_t *>(&c), 1, this);
    if (this->pOwner)
    {
        this->pOwner->Suspend();
    }
}

void UartStream::puts(const char *s)
{
    for (const char *pC = s; pC != nullptr; pC++)
    {
        this->putc(*pC);
    }
}

void UartStream::puts(const char *s, uint16_t len)
{
    this->uart.tx(reinterpret_cast<const std::uint8_t *>(s), len, this);
    if (this->pOwner)
    {
        this->pOwner->Suspend();
    }
}

char UartStream::getc()
{
    char c;
    this->uart.rx(reinterpret_cast<std::uint8_t *>(&c), 1, this);
    if (this->pOwner)
    {
        this->pOwner->Suspend();
    }
    return c;
}

// TODO
// std::optional<char *> UartStream::gets(void)
// {
//     static std::array<char, 0x100> rxBuffer;
//     rxBuffer[rxBuffer.size() - 1] = '\0';

//     for (std::uint16_t i = 0; i < rxBuffer.size() - 1; i++)
//     {
//         rxBuffer[i] = this->getc();

//         if (rxBuffer[i] == '\0')
//         {
//             break;
//         }
//     }

//     return rxBuffer.data();
// }

// std::optional<char *> UartStream::gets(std::uint16_t len)
// {

// }
