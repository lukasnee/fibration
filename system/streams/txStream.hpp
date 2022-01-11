/*
 * Data stream out mechanism
 *
 * Copyright (C) 2021 Lukas Neverauskis <lukas.neverauskis@gmail.com>
 *
 */

#pragma once

#include "ioDataIF.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdio>

class OutStream : public IODataIF::IsrTxCallbacks
{
public:
    using Data = std::uint8_t;

    struct Config
    {
        static constexpr std::size_t txStreamBufferSize = 256;
    };

    OutStream(IODataIF &ioData);
    bool init();
    bool in(const std::uint8_t *pData, std::uint32_t size, OsResource::Context context);
    bool deinit();

protected:
    virtual void onTxCompleteFromIsr() override; // callback

private:
    bool out(OsResource::Context context);

    void reset();
    bool isEmpty();
    std::size_t unbrokenSizeLeft();

    std::array<std::uint8_t, Config::txStreamBufferSize * sizeof(Data)> buffer;
    std::size_t headIdx = 0;
    std::size_t tailIdx = 0;
    bool isRolledOver = false;
    std::size_t activeTxSize = 0;
    TaskHandle_t taskHandleToNotify = nullptr;

    IODataIF &ioData;
};
