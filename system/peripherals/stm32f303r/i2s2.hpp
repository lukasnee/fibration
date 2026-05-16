// Copyright (c) 2026 Lukas Neverauskis <lukas.neverauskis@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only

#pragma once

#include "i2sIF.hpp"
#include <cstdint>

class I2s2 : public I2sIF {
public:
    static I2s2 &getInstance();

    bool init(const I2sIF::Config &config) final;
    bool deinit() final;

    Config get_config() const final;

    bool txRxCircularDmaUnsafe(const std::uint16_t *pTxData16,
                               std::uint16_t *pRxData16,
                               std::uint16_t size) final;
    bool txRxCircularDmaStopUnsafe() final;

private:
    I2s2() = default;
    virtual ~I2s2() = default;
    void operator=(I2s2 const &) = delete;
    I2s2(I2s2 const &) = delete;

    static bool validate_config(const I2sIF::Config &config);

    I2sIF::Config config;
};
