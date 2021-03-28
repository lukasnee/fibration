#include "stereoAudioStreamInterface.hpp"

bool StereoAudioStreamInterface::txStart()
{
    bool retval = false;

    if (this->state == State::standby)
    {
        if (this->txStartUnsafe())
        {
            this->state = State::ready;
            xTaskNotifyGive(this->pOwner->GetHandle());
            retval = true;
        }
    }

    return retval;
}

bool StereoAudioStreamInterface::txStop()
{
    bool retval = false;

    if (this->state == State::firstStreamingSecondReady ||
        this->state == State::firstStreamingSecondLoading ||
        this->state == State::firstStreamingSecondLoaded)
    {
        retval = this->txStopUnsafe();
        this->state = State::standby;
    }
    else if (this->state == State::firstReadySecondStreaming ||
             this->state == State::firstLoadingSecondStreaming ||
             this->state == State::firstLoadedSecondStreaming)
    {
        retval = this->txStopUnsafe();
        this->state = State::standby;
    }

    return retval;
}

std::uint16_t *StereoAudioStreamInterface::getCircularBuffer()
{
    return reinterpret_cast<std::uint16_t *>(&circularBuffer);
}

std::size_t StereoAudioStreamInterface::getCircularBufferSize()
{
    return (sizeof(circularBuffer) / sizeof(decltype(StereoSample::left)));
}

StereoAudioStreamInterface::Buffer *StereoAudioStreamInterface::getStereoAudioBuffer()
{
    Buffer *pStereoAudioBuffer = nullptr;

    if (this->state == State::firstStreamingSecondReady)
    {
        this->state = State::firstStreamingSecondLoading;
        pStereoAudioBuffer = &this->circularBuffer.at(1);
    }
    else if (this->state == State::firstReadySecondStreaming)
    {
        this->state = State::firstLoadingSecondStreaming;
        pStereoAudioBuffer = &this->circularBuffer.at(0);
    }
    else if (this->state == State::standby)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    }

    if (this->state == State::ready)
    {
        this->state = State::firstStandbySecondLoading;
        pStereoAudioBuffer = &this->circularBuffer.at(1);
    }

    return pStereoAudioBuffer;
}

bool StereoAudioStreamInterface::stereoAudioBufferLoaded()
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

void StereoAudioStreamInterface::firstBufferHalfCompletedStreamIsrCallback()
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

void StereoAudioStreamInterface::secondBufferHalfCompletedStreamIsrCallback()
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
