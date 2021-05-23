#include "uartTextService.hpp"
#include "system.hpp"
#include <cstring>
// TODO:rename to TextStreamService injected with textStreamInterface
UartService::UartService(UartIF &uart, const std::string pcName, uint16_t usStackDepth, UBaseType_t uxPriority)
{
    this->setOwner(this);

    if(Thread::Start() == false)
    {
        FibSys::panic();
    }
}

UartService::~UartService()
{
}

bool UartService::txPush(Blob blob)
{
    return this->txQueue.Enqueue(reinterpret_cast<void*>(&blob), 1000);
}

bool UartService::txPush(const char *pData, bool isDataStatic)
{
    UartService::Blob blob = {.pData = pData, .len = static_cast<std::uint16_t>(std::strlen(pData)), .isDataStatic = isDataStatic};
    return this->txPush(blob);
}

bool UartService::txPush(const char *pData, std::uint16_t len, bool isDataStatic)
{
    UartService::Blob blob = {.pData = pData, .len = len, .isDataStatic = isDataStatic};
    return this->txPush(blob);
}

bool UartService::txPushFromISR(Blob blob)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    return this->txQueue.EnqueueFromISR(reinterpret_cast<void*>(&blob), &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

bool UartService::txPushFromISR(const char *pData, bool isDataStatic)
{
    UartService::Blob blob = {.pData = pData, .len = static_cast<std::uint16_t>(std::strlen(pData)), .isDataStatic = isDataStatic};
    return this->txPushFromISR(blob);
}

bool UartService::txPushFromISR(const char *pData, std::uint16_t len, bool isDataStatic)
{
    UartService::Blob blob = {.pData = pData, .len = len, .isDataStatic = isDataStatic};
    return this->txPushFromISR(blob);
}

void UartService::Run() // task code
{
    if(this->init()) 
    {
        UartService::Blob blob;

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
    this->deinit();
}