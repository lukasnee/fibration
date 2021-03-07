#pragma once

#include "Interfaces/uartInterface.hpp"
#include <cstdint>

class Uart1 : public UartInterface
{
public:
    Uart1(std::uint32_t baudrate = 115200);
    ~Uart1();

    bool init() override;
    bool deinit() override;
    bool txUnsafe(const std::uint8_t *pData, std::uint16_t size) override;
    bool rxUnsafe(std::uint8_t *pData, std::uint16_t size) override;

    void operator=(Uart1 const &) = delete;
    Uart1(Uart1 const &) = delete;

private:
    std::uint32_t baudrate;
};
