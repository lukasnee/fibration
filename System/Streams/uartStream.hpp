#pragma once

#include "dataStreamIF.hpp"
#include "uartIF.hpp"

class UartStream : public DataStreamIF, public UartIF::TxIsrCallbacks, public UartIF::RxIsrCallbacks
{
public:
    UartStream(
        UartIF &uart,
        const char *txTaskName, uint16_t txTaskStackDepth, UBaseType_t txTaskPriority, UBaseType_t txQueueMaxItems,
        UBaseType_t rxQueueMaxItems);

protected:
    bool initTxResource() override;
    void deinitTxResource() override;
    bool initRxResource() override;
    void deinitRxResource() override;

    bool tx(const std::uint8_t *pData, std::uint32_t size) override;
    bool txFromIsr(const std::uint8_t *pData, std::uint32_t size) override;
    bool rx(std::uint8_t *pData, std::uint32_t size) override;
    bool rxFromIsr(std::uint8_t *pData, std::uint32_t size) override;

    UartStream(const UartStream &) = delete;
    UartStream(UartStream &&) = delete;

private:
    void onTxComplete() override;
    void onRxComplete() override;

    bool isTxInitialized = false;
    bool isRxInitialized = false;

    UartIF &uart;
};
