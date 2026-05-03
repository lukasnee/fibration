#include "i2sIF.hpp"

I2sIF::I2sIF() {};

bool I2sIF::startTxRxCircularDma(const std::uint16_t *pTxData16,
                                 std::uint16_t *pRxData16, std::uint16_t size8,
                                 TxRxIsrCallbacks *pTxRxIsrCallbacks) {
    if (!this->txRxCircularDmaUnsafe(pTxData16, pRxData16, size8)) {
        return false;
    }
    this->pTxRxIsrCallbacks = pTxRxIsrCallbacks;
    return true;
}

bool I2sIF::stopTxRxCircularDma() { return this->txRxCircularDmaStopUnsafe(); }

void I2sIF::txRxCpltIsrCallback() {
    if (this->pTxRxIsrCallbacks) {
        this->pTxRxIsrCallbacks->onTxRxCompleteIsrCallback();
    }
}

void I2sIF::txRxHalfCpltIsrCallback() {
    if (this->pTxRxIsrCallbacks) {
        this->pTxRxIsrCallbacks->onTxRxHalfCompleteIsrCallback();
    }
}
