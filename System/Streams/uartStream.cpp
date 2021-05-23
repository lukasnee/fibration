#include "uartStream.hpp"
#include "system.hpp"
#include <array>
#include <cstring>

UartStream::UartStream(
    UartIF &uart,
    const char *txTaskName, uint16_t txTaskStackDepth, UBaseType_t txTaskPriority, UBaseType_t txQueueMaxItems,
    const char *rxTaskName, uint16_t rxTaskStackDepth, UBaseType_t rxTaskPriority, UBaseType_t rxQueueMaxItems)
    : DataStreamIF(txTaskName, txTaskStackDepth, txTaskPriority, txQueueMaxItems,
                   rxTaskName, rxTaskStackDepth, rxTaskPriority, rxQueueMaxItems),
      uart(uart)
{
}

bool UartStream::initTxResource()
{
    bool result = true;

    if (false == this->isTxInitialized && false == this->isRxInitialized)
    {
        result = this->isTxInitialized = this->isRxInitialized = this->uart.init();
    }

    return result;
}

void UartStream::deinitTxResource()
{
    if (true == this->isTxInitialized)
    {
        this->isTxInitialized = false;
        if(false == this->isTxInitialized && false == this->isRxInitialized)
        {
            this->uart.deinit(); // NOTE: result ignored
        }
    }
}

bool UartStream::initRxResource()
{
    return this->initTxResource();
}

void UartStream::deinitRxResource()
{
    if (true == this->isRxInitialized)
    {
        this->isRxInitialized = false;
        if(false == this->isTxInitialized && false == this->isRxInitialized)
        {
            this->uart.deinit(); // NOTE: result ignored
        }
    }
}

bool UartStream::tx(const std::uint8_t *pData, std::uint32_t size)
{
    bool result = this->uart.tx(pData, size, this);
    ulTaskNotifyTakeIndexed(0, pdTRUE, portMAX_DELAY);
    return result;
}

bool UartStream::rx(std::uint8_t *pData, std::uint32_t size)
{
    bool result = this->uart.rx(pData, size, this);
    ulTaskNotifyTakeIndexed(1, pdTRUE, portMAX_DELAY);
    return result;
}

void UartStream::onTxCompleteIsrCallback()
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveIndexedFromISR(this->DataStreamIF::TxStream::GetHandle(), 0, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void UartStream::onRxCompleteIsrCallback()
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveIndexedFromISR(this->DataStreamIF::RxStream::GetHandle(), 1, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}