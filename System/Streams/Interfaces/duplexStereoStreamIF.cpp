#include "duplexStereoStreamIF.hpp"

#include "system.hpp"

DuplexStereoStreamIF::DuplexStereoStreamIF(
    const std::string pcName,
    uint16_t usStackDepth,
    UBaseType_t uxPriority,
    CircularBuffer &circularBufferTx,
    CircularBuffer &circularBufferRx,
    ProcessTxRxBufferF processTxRxBufferF)
    : Thread(pcName, usStackDepth, uxPriority),
      circularBufferTx(circularBufferTx),
      circularBufferRx(circularBufferRx),
      processTxRxBufferF(processTxRxBufferF)
{
    this->setOwner(this);

    if (Thread::Start() == false)
    {
        FIBSYS_PANIC();
    }
};

bool DuplexStereoStreamIF::start()
{
    bool retval = false;

    if (this->state == State::standby)
    {
        if (this->startTxRxCircularDma()) // TODO: make DMA start only after the first buffer half is filled with data
        {
            this->state = State::ready;
            xTaskNotifyGive(this->pOwner->GetHandle());
            retval = true;
        }
    }

    return retval;
}

bool DuplexStereoStreamIF::stop()
{
    bool retval = false;

    if (this->state == State::firstStreamingSecondReady ||
        this->state == State::firstStreamingSecondLoading ||
        this->state == State::firstStreamingSecondLoaded)
    {
        retval = this->stopTxRxCircularDma();
        this->state = State::standby;
    }
    else if (this->state == State::firstReadySecondStreaming ||
             this->state == State::firstLoadingSecondStreaming ||
             this->state == State::firstLoadedSecondStreaming)
    {
        retval = this->stopTxRxCircularDma();
        this->state = State::standby;
    }

    return retval;
}

std::uint16_t *DuplexStereoStreamIF::getCircularBufferTx()
{
    return reinterpret_cast<std::uint16_t *>(&this->circularBufferTx);
}

std::uint16_t *DuplexStereoStreamIF::getCircularBufferRxUnsafe()
{
    return reinterpret_cast<std::uint16_t *>(&this->circularBufferRx);
}

const std::uint16_t *DuplexStereoStreamIF::getCircularBufferRx()
{
    return reinterpret_cast<std::uint16_t *>(&this->circularBufferRx);
}

std::size_t DuplexStereoStreamIF::getCircularBufferSize()
{
    return sizeof(circularBufferTx);
}

bool DuplexStereoStreamIF::getStereoAudioBuffersTxRx(const DuplexStereoStreamIF::Buffer *&pRxBufferOut,
                                                     DuplexStereoStreamIF::Buffer *&pTxBufferOut)
{
    bool result = false;

    pRxBufferOut = pTxBufferOut = nullptr;

    if (this->state == State::firstStreamingSecondReady)
    {
        this->state = State::firstStreamingSecondLoading;
        pRxBufferOut = &this->circularBufferRx.at(1);
        pTxBufferOut = &this->circularBufferTx.at(1);
        result = true;
    }
    else if (this->state == State::firstReadySecondStreaming)
    {
        this->state = State::firstLoadingSecondStreaming;
        pRxBufferOut = &this->circularBufferRx.at(0);
        pTxBufferOut = &this->circularBufferTx.at(0);
        result = true;
    }
    else if (this->state == State::standby)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    }

    if (this->state == State::ready)
    {
        this->state = State::firstStandbySecondLoading;
        pRxBufferOut = &this->circularBufferRx.at(1);
        pTxBufferOut = &this->circularBufferTx.at(1);
        result = true;
    }

    return result;
}

bool DuplexStereoStreamIF::stereoAudioBufferLoaded()
{
    bool retval = false;

    if (this->state == State::firstStreamingSecondLoading)
    {
        this->state = State::firstStreamingSecondLoaded;
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        this->state = State::firstReadySecondStreaming;
        retval = true;
    }
    else if (this->state == State::firstLoadingSecondStreaming)
    {
        this->state = State::firstLoadedSecondStreaming;
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        this->state = State::firstStreamingSecondReady;
        retval = true;
    }
    else if (this->state == State::firstStandbySecondLoading)
    {
        this->state = State::firstStandbySecondLoaded;
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        this->state = State::firstReadySecondStreaming;
        retval = true;
    }

    return retval;
};

void DuplexStereoStreamIF::firstBufferHalfCompletedStreamIsrCallback()
{
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

void DuplexStereoStreamIF::secondBufferHalfCompletedStreamIsrCallback()
{
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

void DuplexStereoStreamIF::Run() // task code
{
    cpp_freertos::Thread::Delay(50); // TODO necessary ???
    if (this->init())
    {
        while (true)
        {
            DuplexStereoStreamIF::Buffer *pTxBuffer = nullptr;
            const DuplexStereoStreamIF::Buffer *pRxBuffer = nullptr;

            if (this->processTxRxBufferF &&
                this->getStereoAudioBuffersTxRx(pRxBuffer, pTxBuffer) &&
                pRxBuffer &&
                pTxBuffer)
            {
                this->processTxRxBufferF(*pRxBuffer, *pTxBuffer);
            }

            this->stereoAudioBufferLoaded();
        }
    }
    this->deinit();
}