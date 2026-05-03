// Copyright (c) 2026 Lukas Neverauskis <lukas.neverauskis@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only

#pragma once

#include <cstdint>

class I2sIF {
public:
    virtual bool init() = 0;
    virtual bool deinit() = 0;

    virtual std::uint32_t getSampleRateInHz() const = 0;
    virtual std::uint32_t getSampleBitDepthInBits() const = 0;
    virtual std::uint32_t getSampleFrameSizeInBytes() const = 0;

    // TODO: refactor: try to remove this struct
    struct TxRxIsrCallbacks {
        virtual void onTxRxCompleteIsrCallback() {};
        virtual void onTxRxHalfCompleteIsrCallback() {};
    };

    void txRxCpltIsrCallback();
    void txRxHalfCpltIsrCallback();

    bool startTxRxCircularDma(const std::uint16_t *pTxData16,
                              std::uint16_t *pRxData16, std::uint16_t size,
                              TxRxIsrCallbacks *pTxRxIsrCallbacks = nullptr);
    bool stopTxRxCircularDma();

protected:
    I2sIF() = default;
    virtual ~I2sIF() = default;

    virtual bool txRxCircularDmaUnsafe(const std::uint16_t *pTxData16,
                                       std::uint16_t *pRxData16,
                                       std::uint16_t size) = 0;
    virtual bool txRxCircularDmaStopUnsafe() = 0;

private:
    TxRxIsrCallbacks *pTxRxIsrCallbacks;
};
