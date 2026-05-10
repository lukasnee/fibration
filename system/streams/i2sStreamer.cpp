// Copyright (c) 2026 Lukas Neverauskis <lukas.neverauskis@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only

#include "i2sStreamer.hpp"

#include "FreeRTOS/Addons/LockGuard.hpp"
#include "dsp/dsp.hpp"
#include "dsp/sample.hpp"

#include "ln/ln.h"

I2sStreamer::I2sStreamer(I2sIF &i2s, const char *taskName,
                         uint16_t usStackDepth, UBaseType_t uxPriority,
                         I2sStreamer::Buffer &dma_buffer, ProcessF processF)
    : Task(uxPriority, usStackDepth, taskName), dma_buffer(dma_buffer),
      processF(processF), i2s(i2s) {}

bool I2sStreamer::init() {
    FreeRTOS::Addons::LockGuard lock_guard(this->public_access_mutex);
    if (this->initialized) {
        return true;
    }
    if (!this->i2s.init()) {
        return false;
    }
    this->initialized = true;
    return this->isValid();
}

bool I2sStreamer::deinit() {
    FreeRTOS::Addons::LockGuard lock_guard(this->public_access_mutex);
    if (!this->initialized) {
        return true;
    }
    if (!this->stop()) {
        return false;
    }
    if (!this->i2s.deinit()) {
        return false;
    }
    this->initialized = false;
    return true;
}

void I2sStreamer::set_fn(ProcessF processF) {
    FreeRTOS::Addons::LockGuard lock_guard(this->public_access_mutex);
    this->processF = processF;
}

bool I2sStreamer::start() {
    FreeRTOS::Addons::LockGuard lock_guard(this->public_access_mutex);
    if (!this->initialized) {
        return false;
    }
    if (this->state == State::running) {
        return true;
    }
    LN_ASSERT_PANIC(this->request.sendToBack(Request::start, 0));
    const auto opt_response = this->response.receive(portMAX_DELAY);
    LN_ASSERT_PANIC(opt_response.has_value());
    return opt_response == Response::started;
}

bool I2sStreamer::stop() {
    FreeRTOS::Addons::LockGuard lock_guard(this->public_access_mutex);
    if (!this->initialized) {
        return false;
    }
    if (this->state == State::stopped) {
        return true;
    }
    LN_ASSERT_PANIC(this->request.sendToBack(Request::stop, 0));
    const auto opt_response = this->response.receive(portMAX_DELAY);
    LN_ASSERT_PANIC(opt_response.has_value());
    return opt_response == Response::stopped;
}

std::pair<Fib::Dsp::I2sSampleBufferU32 &, Fib::Dsp::I2sSampleBufferU32 &>
I2sStreamer::get_buf_to_process() {
    Task::notifyTake(portMAX_DELAY);
    if (this->dma_just_started_streaming_first_half) {
        if (this->curr_streaming != CurrentlyStreaming::second) {
            LN_PANIC();
        }
        this->dma_just_started_streaming_first_half = false;
        // half complete - the second half is now being processed
    }
    if (this->curr_streaming == CurrentlyStreaming::none ||
        this->curr_streaming == CurrentlyStreaming::second) {
        return {this->dma_buffer.rx.first, this->dma_buffer.tx.first};
    }
    return {this->dma_buffer.rx.second, this->dma_buffer.tx.second};
}

void I2sStreamer::onTxRxHalfCompleteIsrCallback() {
    this->curr_streaming = CurrentlyStreaming::second;
    bool higherPriorityTaskWoken = false;
    this->Task::notifyGiveFromISR(higherPriorityTaskWoken);
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}
void I2sStreamer::onTxRxCompleteIsrCallback() {
    this->curr_streaming = CurrentlyStreaming::first;
    bool higherPriorityTaskWoken = false;
    this->Task::notifyGiveFromISR(higherPriorityTaskWoken);
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

void I2sStreamer::clear_i2s_dma_tx_buffer() {
    this->dma_buffer.tx.first.fill(
        {Fib::Dsp::Sample::centerValueOfBitDepth<i2sBitDepth>(),
         Fib::Dsp::Sample::centerValueOfBitDepth<i2sBitDepth>()});
    this->dma_buffer.tx.second.fill(
        {Fib::Dsp::Sample::centerValueOfBitDepth<i2sBitDepth>(),
         Fib::Dsp::Sample::centerValueOfBitDepth<i2sBitDepth>()});
}

void I2sStreamer::process() {
    auto [rx_i2s_dma_buf, tx_i2s_dma_buf] = this->get_buf_to_process();

    // TODO: add alternative user-switcable raw_processF that operates on
    // i2s dma buffers directly (saving the conversion step to/from F32)
    if (this->processF) {
        Fib::Dsp::StereoSampleBufferF32 rx_sample_buf, tx_sample_buf;
        Fib::Dsp::Sample::convert<i2sBitDepth>(rx_i2s_dma_buf, rx_sample_buf);
        this->processF(rx_sample_buf, tx_sample_buf);
        Fib::Dsp::Sample::convert<i2sBitDepth>(tx_sample_buf, tx_i2s_dma_buf);
    }
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
void I2sStreamer::taskFunction() {
    while (true) {
        if (this->state == State::running) {
            const auto opt_request = this->request.receive(0);
            if (!opt_request) {
                this->process();
                continue;
            }
            LN_ASSERT_PANIC(opt_request == Request::stop);
            if (!this->i2s.stopTxRxCircularDma()) {
                LN_ASSERT_PANIC(
                    this->response.sendToBack(Response::failed_to_stop, 0));
                continue;
            }
            this->state = State::stopped;
            this->curr_streaming = CurrentlyStreaming::none;
            LN_ASSERT_PANIC(this->response.sendToBack(Response::stopped, 0));
        }
        if (this->state == State::stopped) {
            const auto opt_request = this->request.receive(
                portMAX_DELAY); // wait indefinitely for start request
            LN_ASSERT_PANIC(opt_request.has_value());
            LN_ASSERT_PANIC(opt_request == Request::start);
            this->clear_i2s_dma_tx_buffer();
            this->state = State::running;
            if (!this->i2s.startTxRxCircularDma(
                    reinterpret_cast<std::uint16_t *>(&this->dma_buffer.tx),
                    reinterpret_cast<std::uint16_t *>(&this->dma_buffer.rx),
                    sizeof(dma_buffer.tx), this)) {
                this->state = State::stopped;
                this->curr_streaming = CurrentlyStreaming::first;
                this->dma_just_started_streaming_first_half = true;
                LN_ASSERT_PANIC(
                    this->response.sendToBack(Response::failed_to_start, 0));
                continue;
            }
            LN_ASSERT_PANIC(this->response.sendToBack(Response::started, 0));
        }
    }
}
