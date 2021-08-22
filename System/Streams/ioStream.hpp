#pragma once

#include "ioDataIF.hpp"
#include "stream_buffer.h"

#include <array>

class IOStream : public IODataIF::IsrCallbacks
{
public:
    using RxData = std::uint8_t;
    using TxData = std::uint8_t;

    struct Config
    {
        static constexpr std::size_t rxStreamBufferSize = 256;
        static constexpr std::size_t txStreamBufferSize = 256;
    };

    IOStream(IODataIF &ioData);

    bool init();

    bool pull(RxData &rxData, TickType_t timeout = portMAX_DELAY);
    bool pullFromISR(RxData &rxData, BaseType_t *pxHigherPriorityTaskWoken);

    bool push(const std::uint8_t *pData, std::uint32_t size, bool isDynamic = false, TickType_t timeout = portMAX_DELAY);
    bool pushFromIsr(const std::uint8_t *pData, std::uint32_t size, bool isDynamic = false);

    void deinit();

protected:
    void onTxCompleteFromIsr() override;
    void onRxCompleteFromIsr() override;

private:
    IOStream(const IOStream &) = delete;
    IOStream(IOStream &&) = delete;

    bool isStarted = false;

    // std ::array<TxData, 32> txData = {0};
    TxData txData = 0;
    RxData rxData = 0;
    StreamBufferHandle_t xRxStreamBuffer;
    StaticStreamBuffer_t xRxStreamBufferStruct;
    const std::size_t xRxTriggerLevel = 1;
    std::array<RxData, Config::rxStreamBufferSize * sizeof(RxData)> ucRxBufferStorage;

    bool isBusy = false;
    StreamBufferHandle_t xTxStreamBuffer;
    StaticStreamBuffer_t xTxStreamBufferStruct;
    const std::size_t xTxTriggerLevel = 1;
    std::array<std::uint8_t, Config::txStreamBufferSize * sizeof(TxData)> ucTxBufferStorage;

    IODataIF &ioData;
};
