#pragma once

#include "thread.hpp"
#include <array>

class StereoAudioStreamInterface
{
public:
    struct StereoSample
    {
        std::uint16_t left;
        std::uint16_t right;
    };

    static constexpr std::size_t stereoAudioBufferSize = 0x40;
    using Buffer = std::array<StereoSample, stereoAudioBufferSize>;

    bool txStart();
    bool txStop();

    Buffer *getStereoAudioBuffer();
    bool stereoAudioBufferLoaded();

    void setOwner(cpp_freertos::Thread *pOwner) { this->pOwner = pOwner; }

protected:
    StereoAudioStreamInterface(){};

    virtual bool txStartUnsafe() = 0; // TODO: better symbol name ?
    virtual bool txStopUnsafe() = 0;

    std::uint16_t *getCircularBuffer();
    std::size_t getCircularBufferSize();

    void firstBufferHalfCompletedStreamIsrCallback();
    void secondBufferHalfCompletedStreamIsrCallback();

private:
    using CircularBuffer = std::array<Buffer, 2>;
    CircularBuffer circularBuffer;

    enum State
    {
        standby = 0,

        ready,

        // starting state
        firstStandbySecondLoading,
        firstStandbySecondLoaded,

        // running states - ping
        firstStreamingSecondReady,
        firstStreamingSecondLoading,
        firstStreamingSecondLoaded,

        // running states - pong
        firstReadySecondStreaming,
        firstLoadingSecondStreaming,
        firstLoadedSecondStreaming,

    } state = State::standby;

    cpp_freertos::Thread *pOwner;
};