#include "i2sStream.hpp"

#include "system.hpp"

I2sStream::I2sStream(I2sIF &i2s, const std::string pcName, uint16_t usStackDepth, UBaseType_t uxPriority,
                     I2sStream::Buffer &buffer, ProcessF processF)
    : Thread(pcName, usStackDepth, uxPriority), buffer(buffer), processF(processF), i2s(i2s) {
    this->setOwner(this);

    if (Thread::Start() == false) {
        FIBSYS_PANIC();
    }
}

bool I2sStream::start() {
    bool retval = false;

    if (this->state == State::standby) {
        // TODO: make DMA start only after the first buffer half is filled with data
        if (this->i2s.startTxRxCircularDma(this->getBufferToStreamOut(), this->getBufferToStreamIn(), this->getBufferSize(),
                                           this)) {
            this->state = State::ready;
            xTaskNotifyGive(this->pOwner->GetHandle());
            retval = true;
        }
    }

    return retval;
}

bool I2sStream::stop() {
    bool retval = false;

    if (this->state == State::firstStreamingSecondReady || this->state == State::firstStreamingSecondLoading ||
        this->state == State::firstStreamingSecondLoaded) {
        retval = this->i2s.stopTxRxCircularDma();
        this->state = State::standby;
    }
    else if (this->state == State::firstReadySecondStreaming || this->state == State::firstLoadingSecondStreaming ||
             this->state == State::firstLoadedSecondStreaming) {
        retval = this->i2s.stopTxRxCircularDma();
        this->state = State::standby;
    }

    return retval;
}

std::uint16_t *I2sStream::getBufferToStreamOut() { return reinterpret_cast<std::uint16_t *>(&this->buffer.tx); }
std::uint16_t *I2sStream::getBufferToStreamIn() { return reinterpret_cast<std::uint16_t *>(&this->buffer.rx); }
std::size_t I2sStream::getBufferSize() { return sizeof(buffer.tx); }

bool I2sStream::getBuffersToProcess(Fib::Dsp::I2sSampleBufferU32 *&pRxI2sBufferOut,
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
    else if (this->state == State::standby) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    }

    if (this->state == State::ready) {
        this->state = State::firstStandbySecondLoading;
        pRxI2sBufferOut = &this->buffer.rx.second;
        pTxI2sBufferOut = &this->buffer.tx.second;
        result = true;
    }
    return result;
}

bool I2sStream::stereoAudioBufferLoaded() {
    bool result = false;
    if (this->state == State::firstStreamingSecondLoading) {
        this->state = State::firstStreamingSecondLoaded;
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        this->state = State::firstReadySecondStreaming;
        result = true;
    }
    else if (this->state == State::firstLoadingSecondStreaming) {
        this->state = State::firstLoadedSecondStreaming;
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        this->state = State::firstStreamingSecondReady;
        result = true;
    }
    else if (this->state == State::firstStandbySecondLoading) {
        this->state = State::firstStandbySecondLoaded;
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        this->state = State::firstReadySecondStreaming;
        result = true;
    }
    return result;
};

void I2sStream::onTxRxHalfCompleteIsrCallback() {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if (this->state == State::firstStreamingSecondLoaded ||  // good case
        this->state == State::firstStreamingSecondLoading || // very bad case
        this->state == State::firstStreamingSecondReady ||   // really bad case
        this->state == State::firstStandbySecondLoaded ||    // good case
        this->state == State::firstStandbySecondLoading)     // very bad case
    {
        this->state = State::firstReadySecondStreaming;
        vTaskNotifyGiveFromISR(this->pOwner->GetHandle(), &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
    // TODO: better handle bad cases
}
void I2sStream::onTxRxCompleteIsrCallback() {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if (this->state == State::firstLoadedSecondStreaming ||  // good case
        this->state == State::firstLoadingSecondStreaming || // very bad case
        this->state == State::firstReadySecondStreaming)     // really bad case
    {
        this->state = State::firstStreamingSecondReady;
        vTaskNotifyGiveFromISR(this->pOwner->GetHandle(), &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
    // TODO: better handle bad cases
}

void I2sStream::Run() // task code
{
    cpp_freertos::Thread::Delay(50); // TODO necessary ???

    if (this->i2s.init()) {
        while (true) {
            Fib::Dsp::I2sSampleBufferU32 *pRxI2sBuffer = nullptr;
            Fib::Dsp::I2sSampleBufferU32 *pTxI2sBuffer = nullptr;
            if (this->processF && this->getBuffersToProcess(pRxI2sBuffer, pTxI2sBuffer) && pRxI2sBuffer && pTxI2sBuffer) {
                // TODO: try optimizing, making processF variants in case converting to/from
                // F32 is unnecessary.

                Fib::Dsp::StereoSampleBufferF32 rxStereoSampleBlock, txStereoSampleBlock;
                Fib::Dsp::Sample::convert<i2sBitDepth>(*pRxI2sBuffer, rxStereoSampleBlock);
                this->processF(rxStereoSampleBlock, txStereoSampleBlock);
                Fib::Dsp::Sample::convert<i2sBitDepth>(txStereoSampleBlock, *pTxI2sBuffer);
            }
            this->stereoAudioBufferLoaded();
        }
    }
    this->i2s.deinit();
}