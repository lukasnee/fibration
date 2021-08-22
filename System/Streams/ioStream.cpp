
#include "ioStream.hpp"
#include "system.hpp"
#include <cstring>

IOStream::IOStream(IODataIF &ioData) : ioData(ioData)
{
}

bool IOStream::init()
{
    bool result = false;

    if (this->isStarted)
    {
        result = true;
    }
    else if (false == this->ioData.init())
    {
    }
    else
    {
        this->xTxStreamBuffer = xStreamBufferCreateStatic(this->ucTxBufferStorage.size(),
                                                          this->xTxTriggerLevel,
                                                          this->ucTxBufferStorage.data(),
                                                          &this->xTxStreamBufferStruct);
        if (this->xTxStreamBuffer)
        {
            this->xRxStreamBuffer = xStreamBufferCreateStatic(this->ucRxBufferStorage.size(),
                                                              this->xRxTriggerLevel,
                                                              this->ucRxBufferStorage.data(),
                                                              &this->xRxStreamBufferStruct);
            if (this->xRxStreamBuffer)
            {
                if (this->ioData.rx(&this->rxData, sizeof(RxData)))
                {
                    result = this->isStarted = true;
                }
            }
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

        vStreamBufferDelete(this->xTxStreamBuffer);
        this->xTxStreamBuffer = nullptr;

        this->ioData.deinit();

        this->isStarted = false;
    }
}

bool IOStream::pull(RxData &rxData, TickType_t timeout)
{
    bool result = false;

    if (this->isStarted)
    {
        std::size_t xReceivedBytes = xStreamBufferReceive(this->xRxStreamBuffer, &rxData, sizeof(rxData), timeout);
        if (xReceivedBytes > 0)
        {
            result = true;
        }
    }

    return result;
}

bool IOStream::pullFromISR(RxData &rxData, BaseType_t *pxHigherPriorityTaskWoken)
{
    bool result = false;

    if (this->isStarted)
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        std::size_t xReceivedBytes = xStreamBufferReceiveFromISR(this->xRxStreamBuffer,
                                                                 &rxData,
                                                                 sizeof(rxData),
                                                                 &xHigherPriorityTaskWoken);
        if (xReceivedBytes > 0)
        {
            result = true;
        }
        // portYIELD_FROM_ISR(xHigherPriorityTaskWoken); // there is no task to yield
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
        this->ioData.rxFromIsr(&this->rxData, sizeof(this->rxData), this);
    }
}

bool IOStream::push(const std::uint8_t *pData, std::uint32_t size, bool isDynamic, TickType_t timeout)
{
    bool result = false;

    if (this->isStarted)
    {
        if (this->isBusy)
        {
            std::size_t xBytesSent = xStreamBufferSend(this->xTxStreamBuffer,
                                                       reinterpret_cast<const void *>(pData),
                                                       size,
                                                       timeout);
            if (xBytesSent == size)
            {
                result = true;
            }
        }
        else if (this->ioData.tx(pData, size, this))
        {
            this->isBusy = true;
        }
    }

    return result;
}

bool IOStream::pushFromIsr(const std::uint8_t *pData, std::uint32_t size, bool isDynamic)
{
    bool result = false;

    if (this->isStarted)
    {
        if (this->isBusy)
        {
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            std::size_t xBytesSent = xStreamBufferSendFromISR(this->xTxStreamBuffer,
                                                              reinterpret_cast<const void *>(pData),
                                                              size,
                                                              &xHigherPriorityTaskWoken);
            if (xBytesSent == size)
            {
                result = true;
            }
            // portYIELD_FROM_ISR(xHigherPriorityTaskWoken); // there is no task to yield
        }
        else if (this->ioData.tx(pData, size, this))
        {
            this->isBusy = true;
        }
    }

    return result;
}

void IOStream::onTxCompleteFromIsr()
{
    size_t xReceivedBytes;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xReceivedBytes = xStreamBufferReceiveFromISR(this->xTxStreamBuffer,
                                                 &this->txData,
                                                 sizeof(this->txData),
                                                 &xHigherPriorityTaskWoken);
    if (xReceivedBytes == sizeof(this->txData))
    {
        this->ioData.txFromIsr(reinterpret_cast<const uint8_t *>(&this->txData), sizeof(this->txData), this);
    }
    else
    {
        this->isBusy = false;
    }
}
