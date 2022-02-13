#pragma once

/*
    Duplex Stereo Stream Interface
*/

#include "dsp/sample.hpp"

#include "thread.hpp"

#include <array>
#include <functional>

class DuplexStereoStreamIF : cpp_freertos::Thread {
public:
    // TODO: pass SampleRateInHz

    static constexpr std::size_t i2sBitDepth = 24;

    struct Buffer {
        using DmaDoubleBuffer = std::pair<Fib::Dsp::I2sSampleBufferU32, Fib::Dsp::I2sSampleBufferU32>;
        DmaDoubleBuffer rx, tx;
    };

    using ProcessF = std::function<void([[maybe_unused]] const Fib::Dsp::StereoSampleBufferF32 &rxStereoSampleBlock,
                                        [[maybe_unused]] Fib::Dsp::StereoSampleBufferF32 &txStereoSampleBlock)>;

    bool start();
    bool stop();

protected:
    DuplexStereoStreamIF(const std::string pcName, uint16_t usStackDepth, UBaseType_t uxPriority, Buffer &buffer,
                         ProcessF processF);

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
    enum State {
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

    };

    void setOwner(cpp_freertos::Thread *pOwner) { this->pOwner = pOwner; }
    cpp_freertos::Thread *getOwner() { return this->pOwner; }
    virtual void Run() override;

    const std::uint16_t *getCircularBufferRx();
    bool getStereoAudioBuffersTxRxU32(Fib::Dsp::I2sSampleBufferU32 *&pRxStereoBufferU32Out,
                                      Fib::Dsp::I2sSampleBufferU32 *&pTxStereoBufferU32Out);
    bool stereoAudioBufferLoaded();

    Buffer &buffer;
    ProcessF processF;
    State state = State::standby;
    cpp_freertos::Thread *pOwner;
};