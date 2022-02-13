#include "i2sIF.hpp"

I2sIF::I2sIF() : txRxBinarySemaphore(true){};

bool I2sIF::startTxRxCircularDma(const std::uint16_t *pTxData16, std::uint16_t *pRxData16, std::uint16_t size8,
                                 TxRxIsrCallbacks *pTxRxIsrCallbacks)
{
    bool retval = false;

    this->txRxBinarySemaphore.Take();
    if (false == txRxCircularDmaUnsafe(pTxData16, pRxData16, size8))
    {
        this->txRxBinarySemaphore.Give();
    }
    else
    {
        this->pTxRxIsrCallbacks = pTxRxIsrCallbacks;
        retval = true;
    }

    return retval;
}

bool I2sIF::stopTxRxCircularDma()
{
    bool retval = false;

    if (txRxCircularDmaStopUnsafe())
    {
        retval = this->txRxBinarySemaphore.Give();
    }

    return retval;
}

void I2sIF::txRxCpltIsrCallback()
{
    if (this->pTxRxIsrCallbacks)
    {
        this->pTxRxIsrCallbacks->onTxRxCompleteIsrCallback();
    }
}

void I2sIF::txRxHalfCpltIsrCallback()
{
    if (this->pTxRxIsrCallbacks)
    {
        this->pTxRxIsrCallbacks->onTxRxHalfCompleteIsrCallback();
    }
}
