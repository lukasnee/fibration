#pragma once

#include "Interfaces/adcInterface.hpp"
#include <cstdint>

class Adc2 : public AdcInterface
{
public:
    static Adc2 &getInstance();

    bool init() override;
    bool deinit() override;

    std::uint32_t getBitDepth() override;
    std::uint32_t getChannelsTotal() override;
    bool getValueUnsafe(std::uint32_t channelNo, float &value) override;

    void operator=(Adc2 const &) = delete;
    Adc2(Adc2 const &) = delete;
    
private:
    Adc2();
    ~Adc2();
};