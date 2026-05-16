// Copyright (c) 2026 Lukas Neverauskis <lukas.neverauskis@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only

#pragma once

#include <cstdint>

class I2sIF {
public:
    struct Config {
        /**
         * @brief Sample rate in Hertz.
         */
        uint32_t sample_rate_Hz = 44100;

        /**
         * @brief Sample bit depth in bits.
         */
        uint32_t sample_bit_depth = 24;

        /**
         * @brief Sample frame size in bytes.
         */
        uint32_t sample_frame_size = sizeof(uint32_t);
    };

    virtual bool init(const Config &config) = 0;
    virtual bool deinit() = 0;

    virtual Config get_config() const = 0;

    // TODO: refactor: try to remove this struct
    struct TxRxIsrCallbacks {
        virtual void onTxRxCompleteIsrCallback() {};
        virtual void onTxRxHalfCompleteIsrCallback() {};
    };

    void txRxCpltIsrCallback();
    void txRxHalfCpltIsrCallback();

    bool startTxRxCircularDma(const uint16_t *pTxData16, uint16_t *pRxData16,
                              uint16_t size,
                              TxRxIsrCallbacks *pTxRxIsrCallbacks = nullptr);
    bool stopTxRxCircularDma();

protected:
    I2sIF() = default;
    virtual ~I2sIF() = default;

    virtual bool txRxCircularDmaUnsafe(const uint16_t *pTxData16,
                                       uint16_t *pRxData16, uint16_t size) = 0;
    virtual bool txRxCircularDmaStopUnsafe() = 0;

private:
    TxRxIsrCallbacks *pTxRxIsrCallbacks;
};
