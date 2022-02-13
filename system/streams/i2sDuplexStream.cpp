#include "i2sDuplexStream.hpp"

I2sDuplexStream::I2sDuplexStream(I2sIF &i2s, const std::string pcName, uint16_t usStackDepth, UBaseType_t uxPriority,
                                 I2sDuplexStream::Buffer &buffer, ProcessF processF)
    : DuplexStereoStreamIF(pcName, usStackDepth, uxPriority, buffer, processF), i2s(i2s) {}
I2sDuplexStream::~I2sDuplexStream() {}
bool I2sDuplexStream::init() { return this->i2s.init(); }
bool I2sDuplexStream::deinit() { return this->i2s.deinit(); }
bool I2sDuplexStream::startTxRxCircularDma() {
    return (this->i2s.startTxRxCircularDma(this->getCircularBufferTx(), this->getCircularBufferRxUnsafe(),
                                           this->getCircularBufferSize(), this));
}
bool I2sDuplexStream::stopTxRxCircularDma() { return (this->i2s.stopTxRxCircularDma()); }
void I2sDuplexStream::onTxRxHalfCompleteIsrCallback() { this->firstBufferHalfCompletedStreamIsrCallback(); }
void I2sDuplexStream::onTxRxCompleteIsrCallback() { this->secondBufferHalfCompletedStreamIsrCallback(); }