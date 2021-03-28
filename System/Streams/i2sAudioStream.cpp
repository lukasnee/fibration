#include "i2sAudioStream.hpp"

I2sStream::I2sStream(I2sInterface &i2s) : StereoAudioStreamInterface(), i2s(i2s)
{
    this->i2s.init();
}

I2sStream::~I2sStream()
{
    this->i2s.deinit();
}

bool I2sStream::txStartUnsafe()
{
    bool retval = false;

    if (this->i2s.txCircular(this->getCircularBuffer(), this->getCircularBufferSize(), this))
    {
        retval = true;
    }

    return retval;
}

bool I2sStream::txStopUnsafe()
{
    bool retval = false;

    if (this->i2s.txCircularStop())
    {
        retval = true;
    }

    return retval;
}

void I2sStream::onTxHalfCompleteIsrCallback()
{
    this->firstBufferHalfCompletedStreamIsrCallback();
}

void I2sStream::onTxCompleteIsrCallback()
{
    this->secondBufferHalfCompletedStreamIsrCallback();
}
