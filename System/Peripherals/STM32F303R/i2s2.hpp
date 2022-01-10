#pragma once

/*
    Hardware I2S driver (single resource)
*/

#include "i2sIF.hpp"
#include <cstdint>

class I2s2 : public I2sIF
{
public:
    static I2s2 &getInstance();

    std::uint32_t getSampleRateInHz() const override;
    std::uint32_t getSampleBitDepthInBits() const override;
    std::uint32_t getSampleFrameSizeInBytes() const override;

    bool init() override;
    bool deinit() override;

    bool txRxCircularDmaUnsafe(const std::uint16_t *pTxData16, std::uint16_t *pRxData16, std::uint16_t size) override;
    bool txRxCircularDmaStopUnsafe() override;

    void operator=(I2s2 const &) = delete;
    I2s2(I2s2 const &) = delete;

private:
    I2s2() = default;
    virtual ~I2s2() = default;
};