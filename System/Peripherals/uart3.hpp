#pragma once

#include "Interfaces/uartInterface.hpp"
#include <cstdint>

class Uart3 : public UartInterface
{
public:
    Uart3(std::uint32_t baudrate = 115200);
    ~Uart3();

    bool init() override;
    bool deinit() override;
    bool txUnsafe(const std::uint8_t *pData, std::uint16_t size) override;
    bool rxUnsafe(std::uint8_t *pData, std::uint16_t size) override;

    void operator=(Uart3 const &) = delete;
    Uart3(Uart3 const &) = delete;

private:
    std::uint32_t baudrate;
};
