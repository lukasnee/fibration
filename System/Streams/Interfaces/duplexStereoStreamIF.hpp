#pragma once

/* 
    Duplex Stereo Stream Interface
*/

#include "map.hpp"
#include "thread.hpp"
#include <array>

class DuplexStereoStreamIF : cpp_freertos::Thread
{
public:
    static constexpr std::size_t stereoBufferSize = 0x20; // TODO experiment on this one

    struct StereoSampleU32
    {
        std::uint32_t left, right; // 24-bit value in 32-bit frame
    };
    using StereoBufferU32 = std::array<StereoSampleU32, stereoBufferSize>;
    using CircularStereoBufferU32 = std::array<StereoBufferU32, 2>;

    static_assert(stereoBufferSize % Fib::DSP::SampleBlock<Fib::DSP::F32>().size() == 0,
                  "stereoBufferSize must be a multiple of Fib::DSP::SampleBlock<F32>::size() !");
    using SampleBlocksF32 =
        Fib::DSP::SampleBlocks<Fib::DSP::F32, (stereoBufferSize / Fib::DSP::SampleBlock<Fib::DSP::F32>().size())>;

    // TODO: pass SampleRateInHz
    using ProcessRxTxBuffersF32F = void(const SampleBlocksF32 &rxLeftSampleBlocksF32,
                                        const SampleBlocksF32 &rxRightSampleBlocksF32,
                                        SampleBlocksF32 &txLeftSampleBlocksF32,
                                        SampleBlocksF32 &txRightSampleBlocksF32);

    bool start();
    bool stop();

protected:
    DuplexStereoStreamIF(const std::string pcName, uint16_t usStackDepth, UBaseType_t uxPriority,
                         CircularStereoBufferU32 &circularBufferTx, CircularStereoBufferU32 &circularBufferRx,
                         ProcessRxTxBuffersF32F processRxTxBuffersF32F);

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
    bool getStereoAudioBuffersTxRxU32(
        const StereoBufferU32 *&pRxStereoBufferU32Out, StereoBufferU32 *&pTxStereoBufferU32Out);
    bool stereoAudioBufferLoaded();

    CircularStereoBufferU32 &circularStereoBufferTxU32, &circularStereoBufferRxU32;

    ProcessRxTxBuffersF32F *processRxTxBuffersF32F = nullptr;

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