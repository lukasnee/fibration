// Copyright (c) 2026 Lukas Neverauskis <lukas.neverauskis@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only

#pragma once

#include "i2sIF.hpp"
#include "dsp/dsp.hpp"

#include "FreeRTOS/Task.hpp"
#include "FreeRTOS/Mutex.hpp"
#include "FreeRTOS/Queue.hpp"

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
                UBaseType_t uxPriority, I2sStreamer::Buffer &dma_buffer,
                ProcessF processF = nullptr);
    ~I2sStreamer() = default;

    bool init();
    bool deinit();

    void set_fn(ProcessF processF);

    bool start();
    bool stop();

private:
    I2sStreamer(const I2sStreamer &) = delete;
    I2sStreamer(I2sStreamer &&) = delete;

    // TODO: reconsider whether all these enum states are actually needed, maybe
    // we could simplify things
    enum class Request {
        none,
        start,
        stop,
    };

    enum class Response {
        none,
        started,
        stopped,
        failed_to_start,
        failed_to_stop,
    };

    enum class State {
        stopped = 0,
        running
    };

    enum class CurrentlyStreaming {
        none = 0,
        first,
        second,
    } curr_streaming = CurrentlyStreaming::none;

    std::pair<Fib::Dsp::I2sSampleBufferU32 &, Fib::Dsp::I2sSampleBufferU32 &>
    get_buf_to_process();

    void clear_i2s_dma_tx_buffer();
    void process();

    bool stereoAudioBufferLoaded();

    void onTxRxCompleteIsrCallback() final;
    void onTxRxHalfCompleteIsrCallback() final;

    virtual void taskFunction() override;

    bool initialized = false;
    bool dma_just_started_streaming_first_half = false;
    FreeRTOS::StaticQueue<Request, 1> request;
    FreeRTOS::StaticQueue<Response, 1> response;
    FreeRTOS::RecursiveMutex public_access_mutex;
    Buffer &dma_buffer;
    ProcessF processF;
    State state = State::stopped;
    I2sIF &i2s;
};
