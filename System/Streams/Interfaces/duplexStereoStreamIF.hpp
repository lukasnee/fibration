#pragma once

/* 
    Duplex Stereo Stream Interface
*/

#include "thread.hpp"
#include <array>

class DuplexStereoStreamIF : cpp_freertos::Thread
{
public:
    struct StereoSample
    {
        std::uint32_t left;  // 24-bit value in 32-bit frame
        std::uint32_t right; // 24-bit value in 32-bit frame
    };

    static constexpr std::size_t stereoBufferSize = 0x20; // TODO experiment on this one
    using Buffer = std::array<StereoSample, stereoBufferSize>;
    using CircularBuffer = std::array<Buffer, 2>;

    // TODO: pass SampleRateInHz
    using ProcessTxRxBufferF = void(const DuplexStereoStreamIF::Buffer &rxBuffer, DuplexStereoStreamIF::Buffer &txBuffer);

    bool start();
    bool stop();

protected:
    DuplexStereoStreamIF(const std::string pcName,
                                uint16_t usStackDepth,
                                UBaseType_t uxPriority, CircularBuffer &circularBufferTx, CircularBuffer &circularBufferRx,
                                ProcessTxRxBufferF processTxRxBufferF);

    virtual bool init() = 0;
    virtual bool deinit() = 0;

    virtual bool startTxRxCircularDma() = 0;
    virtual bool stopTxRxCircularDma() = 0;

    std::uint16_t *getCircularBufferTx();
    std::uint16_t *getCircularBufferRxUnsafe();
    std::size_t getCircularBufferSize();

    void firstBufferHalfCompletedStreamIsrCallback();
    void secondBufferHalfCompletedStreamIsrCallback();

private:
    void setOwner(cpp_freertos::Thread *pOwner) { this->pOwner = pOwner; }
    cpp_freertos::Thread *getOwner() { return this->pOwner; }

    virtual void Run() override; // thread entry

    const std::uint16_t *getCircularBufferRx();
    bool getStereoAudioBuffersTxRx(const Buffer *&pBufferRx, Buffer *&pBufferTx);
    bool stereoAudioBufferLoaded();

    CircularBuffer &circularBufferTx;
    CircularBuffer &circularBufferRx;

    ProcessTxRxBufferF *processTxRxBufferF = nullptr;

    // TODO group into a struct `CircularDmaTxRxStream` like in ADC2 driver

    enum State
    {
        standby = 0,

        ready,

        // starting state
        firstStandbySecondLoading,
        firstStandbySecondLoaded,

        // running states - ping
        firstStreamingSecondReady,
        firstStreamingSecondLoading,
        firstStreamingSecondLoaded,

        // running states - pong
        firstReadySecondStreaming,
        firstLoadingSecondStreaming,
        firstLoadedSecondStreaming,

    } state = State::standby;

    cpp_freertos::Thread *pOwner;
};