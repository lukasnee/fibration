#pragma once

/*
    I2S full duplex stream implementation
*/

#include "duplexStereoStreamIF.hpp"
#include "i2sIF.hpp"

#include "thread.hpp"

class I2sDuplexStream : public DuplexStereoStreamIF, private I2sIF::TxRxIsrCallbacks
{
public:
    I2sDuplexStream(I2sIF &i2s,
                    const std::string pcName,
                    uint16_t usStackDepth,
                    UBaseType_t uxPriority,
                    DuplexStereoStreamIF::CircularBuffer &circularBufferTx,
                    DuplexStereoStreamIF::CircularBuffer &circularBufferRx,
                    ProcessTxRxBufferF processTxRxBufferF);
    ~I2sDuplexStream();

protected:
    bool init() override;
    bool deinit() override;

private:
    bool startTxRxCircularDma() override;
    bool stopTxRxCircularDma() override;

    void onTxRxCompleteIsrCallback() override;
    void onTxRxHalfCompleteIsrCallback() override;

    I2sDuplexStream(const I2sDuplexStream &) = delete;
    I2sDuplexStream(I2sDuplexStream &&) = delete;

    I2sIF &i2s;
};
