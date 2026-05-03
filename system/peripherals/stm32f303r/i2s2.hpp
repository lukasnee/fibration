// Copyright (c) 2026 Lukas Neverauskis <lukas.neverauskis@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only

#pragma once

#include "i2sIF.hpp"
#include <cstdint>

class I2s2 : public I2sIF {
public:
    static I2s2 &getInstance();

    std::uint32_t getSampleRateInHz() const override;
    std::uint32_t getSampleBitDepthInBits() const override;
    std::uint32_t getSampleFrameSizeInBytes() const override;

    bool init() override;
    bool deinit() override;

    bool txRxCircularDmaUnsafe(const std::uint16_t *pTxData16,
                               std::uint16_t *pRxData16,
                               std::uint16_t size) override;
    bool txRxCircularDmaStopUnsafe() override;

    void operator=(I2s2 const &) = delete;
    I2s2(I2s2 const &) = delete;

private:
    I2s2() = default;
    virtual ~I2s2() = default;
};
