#pragma once

#include "i2sInterface.hpp"
#include <cstdint>

class I2s2 : public I2sInterface
{
public:
    I2s2();
    ~I2s2();

    bool init() override;
    bool deinit() override;
    bool txCircularUnsafe(const std::uint16_t *pData16, std::uint16_t size) override;

    void operator=(I2s2 const &) = delete;
    I2s2(I2s2 const &) = delete;
};