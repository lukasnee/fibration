// Copyright (c) 2026 Lukas Neverauskis <lukas.neverauskis@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only

#pragma once

#include "i2sIF.hpp"
#include "dsp/dsp.hpp"

#include "FreeRTOS/Task.hpp"

#include <functional>

class I2sStreamer : public FreeRTOS::Task, private I2sIF::TxRxIsrCallbacks {
public:
    // TODO: pass SampleRateInHz

    static constexpr std::size_t i2sBitDepth = 24;

    struct Buffer {
        using DmaDoubleBuffer = std::pair<Fib::Dsp::I2sSampleBufferU32,
                                          Fib::Dsp::I2sSampleBufferU32>;
        DmaDoubleBuffer rx, tx;
    };

    using ProcessF = std::function<void(
        const Fib::Dsp::StereoSampleBufferF32 &rxStereoSampleBlock,
        Fib::Dsp::StereoSampleBufferF32 &txStereoSampleBlock)>;

    I2sStreamer(I2sIF &i2s, const char *taskName, uint16_t usStackDepth,
                UBaseType_t uxPriority, I2sStreamer::Buffer &buffer,
                ProcessF processF = nullptr);

    bool init();
    bool set_fn(ProcessF processF);
    bool start();
    bool stop();
    ~I2sStreamer() = default;

private:
    I2sStreamer(const I2sStreamer &) = delete;
    I2sStreamer(I2sStreamer &&) = delete;

    enum State {
        stopped = 0,

        started,

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

    bool getBuffersToProcess(Fib::Dsp::I2sSampleBufferU32 *&pRxI2sBufferOut,
                             Fib::Dsp::I2sSampleBufferU32 *&pTxI2sBufferOut);
    bool stereoAudioBufferLoaded();

    void onTxRxCompleteIsrCallback();
    void onTxRxHalfCompleteIsrCallback();

    virtual void taskFunction() override;

    Buffer &buffer;
    ProcessF processF;
    State state = State::stopped;
    I2sIF &i2s;
};
