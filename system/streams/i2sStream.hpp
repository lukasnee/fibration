#pragma once

/*
    I2S full duplex stream implementation
*/

#include "dsp/sample.hpp"
#include "i2sIF.hpp"
#include "FreeRTOS/Task.hpp"

#include <array>
#include <functional>
#include <string_view>

class I2sStream : public FreeRTOS::Task, private I2sIF::TxRxIsrCallbacks {
public:
    // TODO: pass SampleRateInHz

    static constexpr std::size_t i2sBitDepth = 24;

    struct Buffer {
        using DmaDoubleBuffer = std::pair<Fib::Dsp::I2sSampleBufferU32, Fib::Dsp::I2sSampleBufferU32>;
        DmaDoubleBuffer rx, tx;
    };

    using ProcessF = std::function<void([[maybe_unused]] const Fib::Dsp::StereoSampleBufferF32 &rxStereoSampleBlock,
                                        [[maybe_unused]] Fib::Dsp::StereoSampleBufferF32 &txStereoSampleBlock)>;

    I2sStream(I2sIF &i2s, const std::string_view taskName, uint16_t usStackDepth, UBaseType_t uxPriority,
              I2sStream::Buffer &buffer, ProcessF processF);
    bool start();
    bool stop();
    ~I2sStream() = default;

private:
    I2sStream(const I2sStream &) = delete;
    I2sStream(I2sStream &&) = delete;

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

    std::uint16_t *getBufferToStreamOut();
    std::uint16_t *getBufferToStreamIn();
    static std::size_t getBufferSize();

    void setOwner(FreeRTOS::Task *pOwner) { this->pOwner = pOwner; }
    FreeRTOS::Task *getOwner() { return this->pOwner; }

    bool getBuffersToProcess(Fib::Dsp::I2sSampleBufferU32 *&pRxStereoBufferU32Out,
                             Fib::Dsp::I2sSampleBufferU32 *&pTxStereoBufferU32Out);
    bool stereoAudioBufferLoaded();

    void onTxRxCompleteIsrCallback();
    void onTxRxHalfCompleteIsrCallback();

    virtual void taskFunction() override;

    Buffer &buffer;
    ProcessF processF;
    State state = State::standby;
    FreeRTOS::Task *pOwner;
    I2sIF &i2s;
};
