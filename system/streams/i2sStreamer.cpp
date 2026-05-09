// Copyright (c) 2026 Lukas Neverauskis <lukas.neverauskis@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only

#include "i2sStreamer.hpp"

#include "dsp/dsp.hpp"
#include "dsp/sample.hpp"

I2sStreamer::I2sStreamer(I2sIF &i2s, const char *taskName,
                         uint16_t usStackDepth, UBaseType_t uxPriority,
                         I2sStreamer::Buffer &buffer, ProcessF processF)
    : Task(uxPriority, usStackDepth, taskName), buffer(buffer),
      processF(processF), i2s(i2s) {}

bool I2sStreamer::init() {
    if (!this->i2s.init()) {
        return false;
    }
    return this->isValid();
}

bool I2sStreamer::set_fn(ProcessF processF) {
    if (this->state != State::stopped) {
        return false;
    }
    this->processF = processF;
    return true;
}

bool I2sStreamer::start() {
    if (!this->isValid()) {
        return false;
    }
    if (this->state != State::stopped) {
        return false;
    }
    // TODO: make DMA start only after the first buffer half is filled
    // with data
    if (!this->i2s.startTxRxCircularDma(this->getBufferToStreamOut(),
                                        this->getBufferToStreamIn(),
                                        I2sStreamer::getBufferSize(), this)) {
        return false;
    }
    this->state = State::started;
    this->notifyGive();
    return true;
}

bool I2sStreamer::stop() {
    if (this->state != State::firstStreamingSecondReady &&
        this->state != State::firstStreamingSecondLoading &&
        this->state != State::firstStreamingSecondLoaded &&
        this->state != State::firstReadySecondStreaming &&
        this->state != State::firstLoadingSecondStreaming &&
        this->state != State::firstLoadedSecondStreaming) {
        return false;
    }
    auto retval = this->i2s.stopTxRxCircularDma();
    this->state = State::stopped;
    return retval;
}

std::uint16_t *I2sStreamer::getBufferToStreamOut() {
    return reinterpret_cast<std::uint16_t *>(&this->buffer.tx);
}
std::uint16_t *I2sStreamer::getBufferToStreamIn() {
    return reinterpret_cast<std::uint16_t *>(&this->buffer.rx);
}
std::size_t I2sStreamer::getBufferSize() { return sizeof(buffer.tx); }

bool I2sStreamer::getBuffersToProcess(
    Fib::Dsp::I2sSampleBufferU32 *&pRxI2sBufferOut,
    Fib::Dsp::I2sSampleBufferU32 *&pTxI2sBufferOut) {
    bool result = false;
    pRxI2sBufferOut = pTxI2sBufferOut = nullptr;
    if (this->state == State::firstStreamingSecondReady) {
        this->state = State::firstStreamingSecondLoading;
        pRxI2sBufferOut = &this->buffer.rx.second;
        pTxI2sBufferOut = &this->buffer.tx.second;
        result = true;
    }
    else if (this->state == State::firstReadySecondStreaming) {
        this->state = State::firstLoadingSecondStreaming;
        pRxI2sBufferOut = &this->buffer.rx.first;
        pTxI2sBufferOut = &this->buffer.tx.first;
        result = true;
    }
    else if (this->state == State::stopped) {
        Task::notifyTake(portMAX_DELAY);
    }

    if (this->state == State::started) {
        this->state = State::firstStandbySecondLoading;
        this->notifyGive();
        pRxI2sBufferOut = &this->buffer.rx.second;
        pTxI2sBufferOut = &this->buffer.tx.second;
        result = true;
    }
    return result;
}

bool I2sStreamer::stereoAudioBufferLoaded() {
    bool result = false;
    if (this->state == State::firstStreamingSecondLoading) {
        this->state = State::firstStreamingSecondLoaded;
        Task::notifyTake(portMAX_DELAY);
        this->state = State::firstReadySecondStreaming;
        result = true;
    }
    else if (this->state == State::firstLoadingSecondStreaming) {
        this->state = State::firstLoadedSecondStreaming;
        Task::notifyTake(portMAX_DELAY);
        this->state = State::firstStreamingSecondReady;
        result = true;
    }
    else if (this->state == State::firstStandbySecondLoading) {
        this->state = State::firstStandbySecondLoaded;
        Task::notifyTake(portMAX_DELAY);
        this->state = State::firstReadySecondStreaming;
        result = true;
    }
    return result;
};

void I2sStreamer::onTxRxHalfCompleteIsrCallback() {
    bool higherPriorityTaskWoken = false;
    if (this->state == State::firstStreamingSecondLoaded ||  // good case
        this->state == State::firstStreamingSecondLoading || // very bad case
        this->state == State::firstStreamingSecondReady ||   // really bad case
        this->state == State::firstStandbySecondLoaded ||    // good case
        this->state == State::firstStandbySecondLoading)     // very bad case
    {
        this->state = State::firstReadySecondStreaming;
        this->notifyGiveFromISR(higherPriorityTaskWoken);
        portYIELD_FROM_ISR(higherPriorityTaskWoken);
    }
    // TODO: better handle bad cases
}
void I2sStreamer::onTxRxCompleteIsrCallback() {
    bool higherPriorityTaskWoken = false;
    if (this->state == State::firstLoadedSecondStreaming ||  // good case
        this->state == State::firstLoadingSecondStreaming || // very bad case
        this->state == State::firstReadySecondStreaming)     // really bad case
    {
        this->state = State::firstStreamingSecondReady;
        this->notifyGiveFromISR(higherPriorityTaskWoken);
        portYIELD_FROM_ISR(higherPriorityTaskWoken);
    }
    // TODO: better handle bad cases
}

void I2sStreamer::taskFunction() {
    while (true) {
        if (this->state == State::stopped) {
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
            if (this->state != State::started) {
                break;
            }
        }
        Fib::Dsp::I2sSampleBufferU32 *pRxI2sBuffer = nullptr;
        Fib::Dsp::I2sSampleBufferU32 *pTxI2sBuffer = nullptr;
        if (this->processF &&
            this->getBuffersToProcess(pRxI2sBuffer, pTxI2sBuffer) &&
            pRxI2sBuffer && pTxI2sBuffer) {
            // TODO: try optimizing, making processF variants in case
            // converting to/from F32 is unnecessary.

            Fib::Dsp::StereoSampleBufferF32 rxStereoSampleBlock,
                txStereoSampleBlock;
            Fib::Dsp::Sample::convert<i2sBitDepth>(*pRxI2sBuffer,
                                                   rxStereoSampleBlock);
            this->processF(rxStereoSampleBlock, txStereoSampleBlock);
            Fib::Dsp::Sample::convert<i2sBitDepth>(txStereoSampleBlock,
                                                   *pTxI2sBuffer);
        }
        this->stereoAudioBufferLoaded();
    }

    this->i2s.deinit();
}
