#pragma once

/*
    defines the required interface for I2S implementation and secures as lockable resource in RTOS context.
*/

#include <cstdint>
#include <semaphore.hpp>

class I2sIF
{
public:
    virtual bool init() = 0;
    virtual bool deinit() = 0;

    virtual std::uint32_t getSampleRateInHz() const = 0;
    virtual std::uint32_t getSampleBitDepthInBits() const = 0;
    virtual std::uint32_t getSampleFrameSizeInBytes() const = 0;

    struct TxRxIsrCallbacks
    {
        virtual void onTxRxCompleteIsrCallback(){};
        virtual void onTxRxHalfCompleteIsrCallback(){};
    };

    void txRxCpltIsrCallback();
    void txRxHalfCpltIsrCallback();

    bool startTxRxCircularDma(const std::uint16_t *pTxData16, std::uint16_t *pRxData16, std::uint16_t size,
                              TxRxIsrCallbacks *pTxRxIsrCallbacks = nullptr);
    bool stopTxRxCircularDma();

protected:
    I2sIF();
    virtual ~I2sIF() = default;

    virtual bool txRxCircularDmaUnsafe(const std::uint16_t *pTxData16, std::uint16_t *pRxData16,
                                       std::uint16_t size) = 0;
    virtual bool txRxCircularDmaStopUnsafe() = 0;

private:
    cpp_freertos::BinarySemaphore txRxBinarySemaphore;
    TxRxIsrCallbacks *pTxRxIsrCallbacks;
};