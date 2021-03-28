#pragma once

#include <cstdint>
#include "thread.hpp"
#include "i2sAudioStream.hpp"

class I2sStreamer : public cpp_freertos::Thread, public I2sStream
{
public:
    using ProcessBufferF = void(StereoAudioStreamInterface::Buffer *);

    I2sStreamer(I2sInterface &i2s, const std::string pcName, uint16_t usStackDepth, UBaseType_t uxPriority, ProcessBufferF processBufferF);
    ~I2sStreamer();

    bool start();
    bool stop();

private:
    bool isStandby = true;

    ProcessBufferF *processBufferF = nullptr;

    virtual void Run() override; // thread entry
};
