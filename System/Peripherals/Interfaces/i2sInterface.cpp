#include "i2sInterface.hpp"

I2sInterface::I2sInterface()
{
    this->txRxBinarySemaphore.Give();
};

I2sInterface::~I2sInterface()
{
    this->txRxBinarySemaphore.Take();
};

bool I2sInterface::startTxRxCircularDma(const std::uint16_t *pTxData16,
                                        std::uint16_t *pRxData16,
                                        std::uint16_t size8,
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

bool I2sInterface::stopTxRxCircularDma()
{
    bool retval = false;

    if (txRxCircularDmaStopUnsafe())
    {
        retval = this->txRxBinarySemaphore.Give();
    }

    return retval;
}

void I2sInterface::txRxCpltIsrCalback()
{
    if (this->pTxRxIsrCallbacks)
    {
        this->pTxRxIsrCallbacks->onTxRxCompleteIsrCallback();
    }
}

void I2sInterface::txRxHalfCpltIsrCalback()
{
    if (this->pTxRxIsrCallbacks)
    {
        this->pTxRxIsrCallbacks->onTxRxHalfCompleteIsrCallback();
    }
}
