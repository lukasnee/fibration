#pragma once

#include "Interfaces/stereoAudioStreamInterface.hpp"
#include "../Peripherals/Interfaces/i2sInterface.hpp"

class I2sStream : public StereoAudioStreamInterface, private I2sInterface::TxIsrCallbacks
{
public:
    I2sStream(I2sInterface &i2s);
    ~I2sStream();

private:
    I2sStream(const I2sStream &) = delete;
    I2sStream(I2sStream &&) = delete;
    bool txStartUnsafe() override;
    bool txStopUnsafe() override;
    void onTxCompleteIsrCallback() override;
    void onTxHalfCompleteIsrCallback() override;

    I2sInterface &i2s;
};
