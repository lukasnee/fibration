#pragma once

#include "adcIF.hpp"
#include <cstdint>
#include <array>

class Adc2 : public AdcIF
{
public:
    static Adc2 &getInstance();

    virtual bool init() override;
    virtual bool start() override;
    virtual std::uint32_t getChannelsTotal() override;
    virtual std::uint32_t getBitDepth() override;
    virtual std::uint32_t getFrameBitWidth() override;
    virtual bool stop() override;
    virtual bool deinit() override;

    struct Internal
    {
        bool init();
        bool deinit();

        struct Irq
        {
            void handle();
            void convHalfCpltCallback();
            void convCpltCallback();
        } irq;
    } internal;

protected:
    virtual bool getValueUnsafe(std::size_t channelNo, std::uint32_t &valueOut) override;

private:
    Adc2() = default;

    bool configChannels();
    bool autoCalibrate();

    virtual ~Adc2() = default;

    void operator=(Adc2 const &) = delete;
    Adc2(Adc2 const &) = delete;
};
