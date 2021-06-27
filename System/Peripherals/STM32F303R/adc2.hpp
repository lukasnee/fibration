#pragma once

#include "adcIF.hpp"
#include <cstdint>
#include <array>

class Adc2 : public AdcIF
{
public:
    static Adc2 &getInstance();

    std::uint32_t getChannelsTotal() override;
    std::uint32_t getBitDepth() override;
    std::uint32_t getFrameBitWidth() override;

    bool init() override;
    bool start() override;
    bool stop() override;
    bool deinit() override;

protected:
    bool getValueUnsafe(std::size_t channelNo, float &value) override;

private:
    // TODO: maybe implement flip-flow read/write sequence
    // template <typename SampleFrame, std::size_t samplesTotal>
    // struct CircularDmaRxStream
    // {
    //     using HalfBuffer = std::array<SampleFrame, samplesTotal>;
    //     using Buffer = std::pair<HalfBuffer, HalfBuffer>;

    //     Buffer buffer;

    //     enum State
    //     {
    //         standby = 0,

    //         // starting states
    //         firstDmaBeginSecondStandby,
    //         firstDmaActiveSecondStandby,
    //         firstDmaEndSecondStandby,

    //         // running states - ping
    //         firstReadyToUseSecondDmaBegin,
    //         firstReadyToUseSecondDmaActive,
    //         firstReadyToUseSecondDmaDone,

    //         // running states - pong
    //         firstDmaBeginSecondReadyToUse,
    //         firstDmaActiveSecondReadyToUse,
    //         firstDmaDoneSecondReadyToUse,

    //     } state = State::standby;
    // };

    // CircularDmaRxStream<std::uint32_t, Config::channelsTotal> circularDmaRxStream;

    Adc2();
    ~Adc2();

    void operator=(Adc2 const &) = delete;
    Adc2(Adc2 const &) = delete;
};