#pragma once

#include "ioDataIF.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdio>

class TxStream : public IODataIF::IsrTxCallbacks
{
public:
    using TxData = std::uint8_t;

    struct Config
    {
        static constexpr std::size_t txStreamBufferSize = 256;
    };

    TxStream(IODataIF &ioData);
    bool init();
    bool deinit();

    bool in(const std::uint8_t *pData, std::uint32_t size, TickType_t timeout = portMAX_DELAY);
    bool inFromIsr(const std::uint8_t *pData, std::uint32_t size);
    // bool in(const char *string)
    // {
    //     return this->in(reinterpret_cast<const uint8_t *>(string), strlen(string));
    // }

protected:
    virtual void onTxCompleteFromIsr() override; // callback

private:
    bool in(const std::uint8_t *pData, std::uint32_t size, TickType_t timeout, bool isFromIsr);
    bool out(bool fromIsr);

    void reset();
    bool isEmpty();
    std::size_t unbrokenSizeLeft();

    std::array<std::uint8_t, Config::txStreamBufferSize * sizeof(TxData)> buffer;
    std::size_t headIdx = 0;
    std::size_t tailIdx = 0;
    bool isRolledOver = false;
    std::size_t activeTxSize = 0;
    TaskHandle_t taskHandleToNotify = nullptr;

    IODataIF &ioData;
};
