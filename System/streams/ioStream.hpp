#pragma once

#include "ioDataIF.hpp" // for I/O
#include "txStream.hpp" // for TX queueing

#include "stream_buffer.h" // for RX queueing

#include <array>

class IOStream : public IODataIF::IsrRxCallbacks, public IODataIF::IsrTxCallbacks
{
public:
    using RxData = std::uint8_t;

    struct Config
    {
        static constexpr std::size_t rxStreamBufferSize = 256;
        static constexpr std::size_t txStreamBufferSize = 256;
    };

    IOStream(IODataIF &ioData);

    bool init();

    bool pull(RxData &rxData, TickType_t timeout, OsResource::Context context);
    bool push(const std::uint8_t *pData, std::uint32_t size, OsResource::Context context = OsResource::Context::undefined);

    void deinit();

protected:
    virtual void onRxCompleteFromIsr() override;

private:
    IOStream(const IOStream &) = delete;
    IOStream(IOStream &&) = delete;

    bool isStarted = false;

    RxData rxData = 0;
    StreamBufferHandle_t xRxStreamBuffer;
    StaticStreamBuffer_t xRxStreamBufferStruct;
    const std::size_t xRxTriggerLevel = 1;
    std::array<RxData, Config::rxStreamBufferSize * sizeof(RxData)> ucRxBufferStorage;

    OutStream txQueue;

    IODataIF &ioData;
};
