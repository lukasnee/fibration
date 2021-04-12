#pragma once

/*
    I2S full duplex stream implementation
*/

#include "Interfaces/duplexStereoStreamInterface.hpp"
#include "../Peripherals/Interfaces/i2sInterface.hpp"

#include "thread.hpp"

class I2sDuplexStream : public DuplexStereoStreamInterface, private I2sInterface::TxRxIsrCallbacks
{
public:
    I2sDuplexStream(I2sInterface &i2s,
                    const std::string pcName,
                    uint16_t usStackDepth,
                    UBaseType_t uxPriority,
                    DuplexStereoStreamInterface::CircularBuffer &circularBufferTx,
                    DuplexStereoStreamInterface::CircularBuffer &circularBufferRx,
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

    I2sInterface &i2s;
};
