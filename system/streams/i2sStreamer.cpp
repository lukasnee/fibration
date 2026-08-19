// Copyright (c) 2026 Lukas Neverauskis <lukas.neverauskis@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only

#include "i2sStreamer.hpp"

#include "FreeRTOS/Addons/LockGuard.hpp"
#include "dsp/dsp.hpp"
#include "dsp/sample.hpp"

#include "i2sIF.hpp"
#include "ln/ln.h"

I2sStreamer::I2sStreamer(const char *taskName, uint16_t usStackDepth,
                         UBaseType_t uxPriority)
    : Task(uxPriority, usStackDepth, taskName) {}

bool I2sStreamer::init(const Config &config) {
    if (!validate_config(config)) {
        return false;
    }
    FreeRTOS::Addons::LockGuard lock_guard(this->public_access_mutex);
    if (this->initialized) {
        return true;
    }
    this->config = config;
    if (!this->config.i2s->init(config.i2s_config)) {
        return false;
    }
    this->initialized = true;
    return this->Task::isValid();
}

bool I2sStreamer::validate_config(const Config &config) {
    if (!config.i2s) {
        return false;
    }
    if (!config.dma_buffer) {
        return false;
    }
    return true;
}

bool I2sStreamer::deinit() {
    FreeRTOS::Addons::LockGuard lock_guard(this->public_access_mutex);
    if (!this->initialized) {
        return true;
    }
    if (!this->stop()) {
        return false;
    }
    if (!this->config.i2s->deinit()) {
        return false;
    }
    this->initialized = false;
    return true;
}

void I2sStreamer::set_fn(ProcessF process_fn) {
    FreeRTOS::Addons::LockGuard lock_guard(this->public_access_mutex);
    this->config.process_fn = process_fn;
}

bool I2sStreamer::start() {
    FreeRTOS::Addons::LockGuard lock_guard(this->public_access_mutex);
    if (!this->initialized) {
        return false;
    }
    if (this->state == State::running) {
        return true;
    }
    LN_ASSERT_PANIC(this->task_request.sendToBack(Request::start, 0));
    const auto opt_response = this->task_response.receive(portMAX_DELAY);
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
    LN_ASSERT_PANIC(this->task_request.sendToBack(Request::stop, 0));
    const auto opt_response = this->task_response.receive(portMAX_DELAY);
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
        return {this->config.dma_buffer->rx.first,
                this->config.dma_buffer->tx.first};
    }
    return {this->config.dma_buffer->rx.second,
            this->config.dma_buffer->tx.second};
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

// NOLINTNEXTLINE(readability-make-member-function-const)
void I2sStreamer::clear_i2s_dma_tx_buffer() {
    const auto bit_depth = this->config.i2s_config.sample_bit_depth;
    this->config.dma_buffer->tx.first.fill(
        {Fib::Dsp::Sample::centerValueOfBitDepth(bit_depth),
         Fib::Dsp::Sample::centerValueOfBitDepth(bit_depth)});
    this->config.dma_buffer->tx.second.fill(
        {Fib::Dsp::Sample::centerValueOfBitDepth(bit_depth),
         Fib::Dsp::Sample::centerValueOfBitDepth(bit_depth)});
}

void I2sStreamer::process_audio_buffers() {
    auto [rx_i2s_dma_buf, tx_i2s_dma_buf] = this->get_buf_to_process();

    // TODO: add alternative user-switcable raw_processF that operates on
    // i2s dma buffers directly (saving the conversion step to/from F32)
    if (this->config.process_fn) {
        Fib::Dsp::StereoSampleBufferF32 rx_sample_buf, tx_sample_buf;
        Fib::Dsp::Sample::convert(rx_i2s_dma_buf, rx_sample_buf,
                                  this->config.i2s_config.sample_bit_depth);
        this->config.process_fn(rx_sample_buf, tx_sample_buf);
        Fib::Dsp::Sample::convert(tx_sample_buf, tx_i2s_dma_buf,
                                  this->config.i2s_config.sample_bit_depth);
    }
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
void I2sStreamer::taskFunction() {
    while (true) {
        if (this->state == State::running) {
            const auto opt_request = this->task_request.receive(0);
            if (!opt_request) {
                this->process_audio_buffers();
                continue;
            }
            LN_ASSERT_PANIC(opt_request == Request::stop);
            if (!this->config.i2s->stopTxRxCircularDma()) {
                LN_ASSERT_PANIC(this->task_response.sendToBack(
                    Response::failed_to_stop, 0));
                continue;
            }
            this->state = State::stopped;
            this->curr_streaming = CurrentlyStreaming::none;
            LN_ASSERT_PANIC(
                this->task_response.sendToBack(Response::stopped, 0));
        }
        if (this->state == State::stopped) {
            const auto opt_request = this->task_request.receive(
                portMAX_DELAY); // wait indefinitely for start request
            LN_ASSERT_PANIC(opt_request.has_value());
            LN_ASSERT_PANIC(opt_request == Request::start);
            this->clear_i2s_dma_tx_buffer();
            this->state = State::running;
            if (!this->config.i2s->startTxRxCircularDma(
                    reinterpret_cast<std::uint16_t *>(
                        &this->config.dma_buffer->tx),
                    reinterpret_cast<std::uint16_t *>(
                        &this->config.dma_buffer->rx),
                    sizeof(this->config.dma_buffer->tx), this)) {
                this->state = State::stopped;
                this->curr_streaming = CurrentlyStreaming::first;
                this->dma_just_started_streaming_first_half = true;
                LN_ASSERT_PANIC(this->task_response.sendToBack(
                    Response::failed_to_start, 0));
                continue;
            }
            LN_ASSERT_PANIC(
                this->task_response.sendToBack(Response::started, 0));
        }
    }
}
