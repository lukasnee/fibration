#include "duplexStereoStreamIF.hpp"

#include "system.hpp"

DuplexStereoStreamIF::DuplexStereoStreamIF(
    const std::string pcName,
    uint16_t usStackDepth,
    UBaseType_t uxPriority,
    CircularStereoBufferU32 &circularStereoBufferTxU32,
    CircularStereoBufferU32 &circularStereoBufferRxU32,
    ProcessRxTxBuffersF32F processRxTxBuffersF32F)
    : Thread(pcName, usStackDepth, uxPriority),
      circularStereoBufferTxU32(circularStereoBufferTxU32),
      circularStereoBufferRxU32(circularStereoBufferRxU32),
      processRxTxBuffersF32F(processRxTxBuffersF32F)
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
    return reinterpret_cast<std::uint16_t *>(&this->circularStereoBufferTxU32);
}

std::uint16_t *DuplexStereoStreamIF::getCircularBufferRxUnsafe()
{
    return reinterpret_cast<std::uint16_t *>(&this->circularStereoBufferRxU32);
}

const std::uint16_t *DuplexStereoStreamIF::getCircularBufferRx()
{
    return reinterpret_cast<std::uint16_t *>(&this->circularStereoBufferRxU32);
}

std::size_t DuplexStereoStreamIF::getCircularBufferSize()
{
    return sizeof(circularStereoBufferTxU32);
}

bool DuplexStereoStreamIF::getStereoAudioBuffersTxRxU32(const DuplexStereoStreamIF::StereoBufferU32 *&pRxStereoBufferU32Out,
                                                        DuplexStereoStreamIF::StereoBufferU32 *&pTxStereoBufferU32Out)
{
    bool result = false;

    pRxStereoBufferU32Out = pTxStereoBufferU32Out = nullptr;

    if (this->state == State::firstStreamingSecondReady)
    {
        this->state = State::firstStreamingSecondLoading;
        pRxStereoBufferU32Out = &this->circularStereoBufferRxU32.at(1);
        pTxStereoBufferU32Out = &this->circularStereoBufferTxU32.at(1);
        result = true;
    }
    else if (this->state == State::firstReadySecondStreaming)
    {
        this->state = State::firstLoadingSecondStreaming;
        pRxStereoBufferU32Out = &this->circularStereoBufferRxU32.at(0);
        pTxStereoBufferU32Out = &this->circularStereoBufferTxU32.at(0);
        result = true;
    }
    else if (this->state == State::standby)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    }

    if (this->state == State::ready)
    {
        this->state = State::firstStandbySecondLoading;
        pRxStereoBufferU32Out = &this->circularStereoBufferRxU32.at(1);
        pTxStereoBufferU32Out = &this->circularStereoBufferTxU32.at(1);
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

            const DuplexStereoStreamIF::StereoBufferU32 *pRxStereoBufferU32 = nullptr;
            DuplexStereoStreamIF::StereoBufferU32 *pTxStereoBufferU32 = nullptr;

            if (this->processRxTxBuffersF32F &&
                this->getStereoAudioBuffersTxRxU32(pRxStereoBufferU32, pTxStereoBufferU32) &&
                pRxStereoBufferU32 && pTxStereoBufferU32)
            {
                DuplexStereoStreamIF::SampleBlocksF32
                    rxLeftSampleBlocksF32,
                    rxRightSampleBlocksF32,
                    txLeftSampleBlocksF32,
                    txRightSampleBlocksF32;

                // TODO: try optimizing, making processRxTxBuffersF32F variants in case converting to/from F32 is unnecessary.
                // TODO: maybe move out conversion functions to user space (like done with processRxTxBuffersF32F)
                /** @note ADC actually support 24-bit samples in 32-bit frame therefore shifting sample by 8 bits is necessary */

                /* Rx buffer: U32->F32 */
                for (std::size_t i = 0; i < rxLeftSampleBlocksF32.size(); i++)
                {
                    for (std::size_t j = 0; j < rxLeftSampleBlocksF32.front().size(); j++)
                    {
                        rxLeftSampleBlocksF32[i][j] = Fib::DSP::floatToQ31((Fib::DSP::swap((*pRxStereoBufferU32)[i * rxLeftSampleBlocksF32.front().size() + j].left) << 8));
                        rxRightSampleBlocksF32[i][j] = Fib::DSP::floatToQ31((Fib::DSP::swap((*pRxStereoBufferU32)[i * rxRightSampleBlocksF32.front().size() + j].right) << 8));
                    }
                }

                this->processRxTxBuffersF32F(rxLeftSampleBlocksF32, rxRightSampleBlocksF32, txLeftSampleBlocksF32, txRightSampleBlocksF32);

                /* Tx buffer: F32->U32 */
                for (std::size_t i = 0; i < txLeftSampleBlocksF32.size(); i++)
                {
                    for (std::size_t j = 0; j < txLeftSampleBlocksF32.front().size(); j++)
                    {
                        (*pTxStereoBufferU32)[i * rxLeftSampleBlocksF32.front().size() + j].left = Fib::DSP::swap(Fib::DSP::floatToQ31(txLeftSampleBlocksF32[i][j]) >> 8);
                        (*pTxStereoBufferU32)[i * rxLeftSampleBlocksF32.front().size() + j].right = Fib::DSP::swap(Fib::DSP::floatToQ31(txRightSampleBlocksF32[i][j]) >> 8);
                    }
                }
            }

            this->stereoAudioBufferLoaded();
        }
    }
    this->deinit();
}