// Copyright (c) 2026 Lukas Neverauskis <lukas.neverauskis@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only

#pragma once

#include "i2sIF.hpp"
#include "dsp/dsp.hpp"

#include "FreeRTOS/Task.hpp"
#include "FreeRTOS/Mutex.hpp"
#include "FreeRTOS/Queue.hpp"

#include <functional>

class I2sStreamer : public FreeRTOS::Task, private I2sIF::TxRxIsrCallbacks {
public:
    struct Buffer {
        using DmaDoubleBuffer = std::pair<Fib::Dsp::I2sSampleBufferU32,
                                          Fib::Dsp::I2sSampleBufferU32>;
        DmaDoubleBuffer rx, tx;
    };

    using ProcessF = std::function<void(
        const Fib::Dsp::StereoSampleBufferF32 &rxStereoSampleBlock,
        Fib::Dsp::StereoSampleBufferF32 &txStereoSampleBlock)>;

    struct Config {
        I2sIF *i2s;
        I2sIF::Config i2s_config;
        // TODO: make dma_buffer adjustable (std::span, etc.)
        I2sStreamer::Buffer *dma_buffer;
        ProcessF process_fn = nullptr;
    };

    I2sStreamer(const char *taskName, uint16_t usStackDepth,
                UBaseType_t uxPriority);
    ~I2sStreamer() = default;

    bool init(const Config &config);
    bool deinit();

    void set_fn(ProcessF process_fn);

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

    static bool validate_config(const Config &config);
    void clear_i2s_dma_tx_buffer();
    void process();

    bool stereoAudioBufferLoaded();

    void onTxRxCompleteIsrCallback() final;
    void onTxRxHalfCompleteIsrCallback() final;

    virtual void taskFunction() override;

    bool initialized = false;
    bool dma_just_started_streaming_first_half = false;
    State state = State::stopped;
    FreeRTOS::StaticQueue<Request, 1> task_request;
    FreeRTOS::StaticQueue<Response, 1> task_response;
    FreeRTOS::RecursiveMutex public_access_mutex;

    Config config;
};
