#include "uartTextStreamer.hpp"
#include "system.hpp"
#include <cstring>

UartStreamer::UartStreamer(UartInterface &uart, const std::string pcName, uint16_t usStackDepth, UBaseType_t uxPriority)
    : Thread{pcName, usStackDepth, uxPriority}, UartStream(uart), txQueue(0x20, sizeof(UartStreamer::Blob))
{
    this->setOwner(this);

    if(Thread::Start() == false)
    {
        FibSys::panic();
    }
}

UartStreamer::~UartStreamer()
{
}

bool UartStreamer::txPush(Blob blob)
{
    return this->txQueue.Enqueue(reinterpret_cast<void*>(&blob), 1000);
}

bool UartStreamer::txPush(const char *pData, bool isDataStatic)
{
    UartStreamer::Blob blob = {.pData = pData, .len = static_cast<std::uint16_t>(std::strlen(pData)), .isDataStatic = isDataStatic};
    return this->txPush(blob);
}

bool UartStreamer::txPush(const char *pData, std::uint16_t len, bool isDataStatic)
{
    UartStreamer::Blob blob = {.pData = pData, .len = len, .isDataStatic = isDataStatic};
    return this->txPush(blob);
}

bool UartStreamer::txPushFromISR(Blob blob)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    return this->txQueue.EnqueueFromISR(reinterpret_cast<void*>(&blob), &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

bool UartStreamer::txPushFromISR(const char *pData, bool isDataStatic)
{
    UartStreamer::Blob blob = {.pData = pData, .len = static_cast<std::uint16_t>(std::strlen(pData)), .isDataStatic = isDataStatic};
    return this->txPushFromISR(blob);
}

bool UartStreamer::txPushFromISR(const char *pData, std::uint16_t len, bool isDataStatic)
{
    UartStreamer::Blob blob = {.pData = pData, .len = len, .isDataStatic = isDataStatic};
    return this->txPushFromISR(blob);
}

void UartStreamer::Run() // task code
{
    UartStreamer::Blob blob;

    while (true)
    {
        if(this->txQueue.Dequeue(reinterpret_cast<void*>(&blob)))
        {
            this->puts(blob.pData, blob.len);
            if (blob.isDataStatic == false)
            {
                delete[] blob.pData;
            }
        }
    }
}