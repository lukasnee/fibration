#include "duplexStereoStreamInterface.hpp"

#include "system.hpp"

DuplexStereoStreamInterface::DuplexStereoStreamInterface(const std::string pcName,
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
        FibSys::panic();
    }
};

bool DuplexStereoStreamInterface::start()
{
    bool retval = false;

    if (this->state == State::standby)
    {
        if (this->startTxRxCircularDma())
        {
            this->state = State::ready;
            xTaskNotifyGive(this->pOwner->GetHandle());
            retval = true;
        }
    }

    return retval;
}

bool DuplexStereoStreamInterface::stop()
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

std::uint16_t *DuplexStereoStreamInterface::getCircularBufferTx()
{
    return reinterpret_cast<std::uint16_t *>(&this->circularBufferTx);
}

std::uint16_t *DuplexStereoStreamInterface::getCircularBufferRxUnsafe()
{
    return reinterpret_cast<std::uint16_t *>(&this->circularBufferRx);
}

const std::uint16_t *DuplexStereoStreamInterface::getCircularBufferRx()
{
    return reinterpret_cast<std::uint16_t *>(&this->circularBufferRx);
}

std::size_t DuplexStereoStreamInterface::getCircularBufferSize()
{
    return sizeof(circularBufferTx);
}

bool DuplexStereoStreamInterface::getStereoAudioBuffersTxRx(const DuplexStereoStreamInterface::Buffer *&pRxBuffer,
                                                            DuplexStereoStreamInterface::Buffer *&pTxBuffer)
{
    bool result = false;

    if (this->state == State::firstStreamingSecondReady)
    {
        this->state = State::firstStreamingSecondLoading;
        pRxBuffer = &this->circularBufferRx.at(1);
        pTxBuffer = &this->circularBufferTx.at(1);
        result = true;
    }
    else if (this->state == State::firstReadySecondStreaming)
    {
        this->state = State::firstLoadingSecondStreaming;
        pRxBuffer = &this->circularBufferRx.at(0);
        pTxBuffer = &this->circularBufferTx.at(0);
        result = true;
    }
    else if (this->state == State::standby)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    }

    if (this->state == State::ready)
    {
        this->state = State::firstStandbySecondLoading;
        pRxBuffer = &this->circularBufferRx.at(1);
        pTxBuffer = &this->circularBufferTx.at(1);
        result = true;
    }

    return result;
}

bool DuplexStereoStreamInterface::stereoAudioBufferLoaded()
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

void DuplexStereoStreamInterface::firstBufferHalfCompletedStreamIsrCallback()
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

void DuplexStereoStreamInterface::secondBufferHalfCompletedStreamIsrCallback()
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

void DuplexStereoStreamInterface::Run() // task code
{
    cpp_freertos::Thread::Delay(50);
    if (this->init())
    {
        while (true)
        {
            DuplexStereoStreamInterface::Buffer *pTxBuffer = nullptr;
            const DuplexStereoStreamInterface::Buffer *pRxBuffer = nullptr;

            if (this->processTxRxBufferF &&
                this->getStereoAudioBuffersTxRx(pRxBuffer, pTxBuffer) &&
                pRxBuffer &&
                pTxBuffer)
            {
                this->processTxRxBufferF(pRxBuffer, pTxBuffer);
            }

            this->stereoAudioBufferLoaded();
        }
    }
    this->deinit();
}