#pragma once

#include "dataStreamIF.hpp"
#include "uartIF.hpp"

class UartStream : public DataStreamIF, public UartIF::TxIsrCallbacks, public UartIF::RxIsrCallbacks
{
public:
    UartStream(UartInterface &uart);
    ~UartStream();

    bool init();
    bool deinit();

    void putc(char c) override;
    void puts(const char *s) override;
    void puts(const char *s, std::uint16_t len) override;

    char getc() override;
    // TODO
    // std::optional<char *> gets(void) override;
    // std::optional<char *> gets(std::uint16_t len) override;

    UartStream(const UartStream &) = delete;
    UartStream(UartStream &&) = delete;

private:
    void onTxCompleteIsrCallback() override;
    void onRxCompleteIsrCallback() override;

    UartInterface &uart;
};
