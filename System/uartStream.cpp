#include "uartStream.hpp"
#include "system.hpp"
#include <array>
#include <cstring>

UartStream::UartStream(UartInterface &uart) : Stream(), uart(uart)
{
    this->uart.init();
}

UartStream::~UartStream()
{
    this->uart.deinit();
}

void UartStream::onTxCompleteIsrCallback()
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveIndexedFromISR(this->pOwner->GetHandle(), 0, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void UartStream::onRxCompleteIsrCallback()
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveIndexedFromISR(this->pOwner->GetHandle(), 1, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void UartStream::putc(char c)
{
    this->uart.tx(reinterpret_cast<std::uint8_t *>(&c), 1, this);
    ulTaskNotifyTakeIndexed(0, pdTRUE, portMAX_DELAY);
}

void UartStream::puts(const char *s, uint16_t len)
{
    this->uart.tx(reinterpret_cast<const std::uint8_t *>(s), len, this);
    ulTaskNotifyTakeIndexed(0, pdTRUE, portMAX_DELAY);
}

void UartStream::puts(const char *s)
{
    puts(s, std::strlen(s));
}

char UartStream::getc()
{
    char c;
    this->uart.rx(reinterpret_cast<std::uint8_t *>(&c), 1, this);
    ulTaskNotifyTakeIndexed(1, pdTRUE, portMAX_DELAY);
    return c;
}

// TODO: whats the best way to buffer this in ? + use std optional
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
