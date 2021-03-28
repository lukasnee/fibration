
#include "i2sAudioStreamer.hpp"
#include "system.hpp"
#include <cstring>

I2sStreamer::I2sStreamer(I2sInterface &i2s, const std::string pcName, uint16_t usStackDepth, UBaseType_t uxPriority, ProcessBufferF processBufferF)
    : Thread{pcName, usStackDepth, uxPriority}, I2sStream(i2s), processBufferF(processBufferF)
{
    this->setOwner(this);

    if (Thread::Start() == false)
    {
        FibSys::panic();
    }
}

I2sStreamer::~I2sStreamer()
{
    this->stop();
}

bool I2sStreamer::start()
{
    if (this->txStart())
    {
        return true;
    }
    return false;
}

bool I2sStreamer::stop()
{
    if (this->txStop())
    {
        return true;
    }
    return false;
}

void I2sStreamer::Run() // task code
{
    while (true)
    {
        StereoAudioStreamInterface::Buffer *pStereoAudioBuffer = this->getStereoAudioBuffer();

        if (processBufferF && pStereoAudioBuffer)
        {
            this->processBufferF(pStereoAudioBuffer);
        }

        this->stereoAudioBufferLoaded();
    }
}