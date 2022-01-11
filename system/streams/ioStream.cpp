
#include "ioStream.hpp"
#include "system.hpp"
#include <cstring>

IOStream::IOStream(IODataIF &ioData) : txQueue(ioData), ioData(ioData)
{
}

bool IOStream::init()
{
    bool result = false;

    if (this->isStarted)
    {
        result = true;
    }
    else if (this->txQueue.init()) /* inits `ioData` too */
    {
        this->xRxStreamBuffer = xStreamBufferCreateStatic(this->ucRxBufferStorage.size(),
                                                          this->xRxTriggerLevel,
                                                          this->ucRxBufferStorage.data(),
                                                          &this->xRxStreamBufferStruct);
        if (this->xRxStreamBuffer)
        {
            if (this->ioData.rxDma(&this->rxData, sizeof(RxData), this, OsResource::Context::task))
            {
                result = this->isStarted = true;
            }
        }
        else
        {
            this->txQueue.deinit();
        }
    }

    return result;
}

void IOStream::deinit()
{
    if (this->isStarted)
    {
        vStreamBufferDelete(this->xRxStreamBuffer);
        this->xRxStreamBuffer = nullptr;
        this->txQueue.deinit();
        this->ioData.deinit();
        this->isStarted = false;
    }
}

bool IOStream::pull(RxData &rxData, TickType_t timeout, OsResource::Context context = OsResource::Context::undefined)
{
    bool result = false;

    if (this->isStarted)
    {

        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        std::size_t xReceivedBytes = OsResource::isInIsr(context) ? xStreamBufferReceiveFromISR(
                                                                        this->xRxStreamBuffer,
                                                                        &rxData,
                                                                        sizeof(rxData),
                                                                        &xHigherPriorityTaskWoken)
                                                                  : xStreamBufferReceive(
                                                                        this->xRxStreamBuffer,
                                                                        &rxData,
                                                                        sizeof(rxData),
                                                                        timeout);
        if (xReceivedBytes > 0)
        {
            result = true;
        }
    }

    return result;
}

void IOStream::onRxCompleteFromIsr()
{
    size_t xBytesSent;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xBytesSent = xStreamBufferSendFromISR(this->xRxStreamBuffer,
                                          &this->rxData,
                                          sizeof(this->rxData),
                                          &xHigherPriorityTaskWoken);

    if (xBytesSent == sizeof(this->rxData))
    {
        this->ioData.rxDma(&this->rxData, sizeof(this->rxData), this, OsResource::Context::isr);
    }
}

bool IOStream::push(const std::uint8_t *pData, std::uint32_t size, OsResource::Context context)
{
    bool result = false;

    if (this->isStarted)
    {
        if (this->txQueue.in(pData, size, context))
        {
            result = true;
        }
    }

    return result;
}
