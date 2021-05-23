#include "i2sDuplexStream.hpp"

I2sDuplexStream::I2sDuplexStream(I2sIF &i2s,
                                 const std::string pcName,
                                 uint16_t usStackDepth,
                                 UBaseType_t uxPriority,
                                 CircularBuffer &circularBufferTx,
                                 CircularBuffer &circularBufferRx,
                                 ProcessTxRxBufferF processTxRxBufferF)
    : DuplexStereoStreamIF(pcName, usStackDepth, uxPriority, circularBufferTx, circularBufferRx, processTxRxBufferF),
      i2s(i2s) {}

I2sDuplexStream::~I2sDuplexStream() {}

bool I2sDuplexStream::init() { return this->i2s.init(); }

bool I2sDuplexStream::deinit() { return this->i2s.deinit(); }

bool I2sDuplexStream::startTxRxCircularDma()
{
    return (this->i2s.startTxRxCircularDma(this->getCircularBufferTx(),
                                           this->getCircularBufferRxUnsafe(),
                                           this->getCircularBufferSize(),
                                           this));
}

bool I2sDuplexStream::stopTxRxCircularDma() { return (this->i2s.stopTxRxCircularDma()); }

// callback routing:

void I2sDuplexStream::onTxRxHalfCompleteIsrCallback() { this->firstBufferHalfCompletedStreamIsrCallback(); }

void I2sDuplexStream::onTxRxCompleteIsrCallback() { this->secondBufferHalfCompletedStreamIsrCallback(); }
