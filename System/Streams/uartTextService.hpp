#pragma once

#include <cstdint>
#include "thread.hpp"
#include "queue.hpp"
#include "uartTextStream.hpp"

class UartService : public UartStream, public cpp_freertos::Thread
{
public:
    UartService(UartIF &uart, const std::string pcName, uint16_t usStackDepth, UBaseType_t uxPriority);
    ~UartService();

    struct Blob
    {
        const char *pData = nullptr;
        std::uint16_t len = 0;
        bool isDataStatic = false; // TODO: really neccessary?
    };

    bool txPush(Blob blob);
    bool txPush(const char *pData, bool isDataStatic = false);
    bool txPush(const char *pData, std::uint16_t len, bool isDataStatic = false);
    bool txPushFromISR(Blob blob);
    bool txPushFromISR(const char *pData, bool isDataStatic = false);
    bool txPushFromISR(const char *pData, std::uint16_t len, bool isDataStatic = false);

private:
    virtual void Run() override; // thread entry
    cpp_freertos::Queue txQueue; //TODO: rxQueue?
};
