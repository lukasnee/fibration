#pragma once

#include "ioDataIF.hpp"
#include <cstdint>

class Uart2 : public IODataIF
{
public:
    Uart2(std::uint32_t baudrate = 115200);
    ~Uart2();

protected:
    bool initUnsafe() override;
    bool deinitUnsafe() override;
    bool txUnsafe(const std::uint8_t *pData, std::size_t size) override;
    bool rxUnsafe(std::uint8_t *pData, std::size_t size) override;

    void operator=(Uart2 const &) = delete;
    Uart2(Uart2 const &) = delete;

private:
    std::uint32_t baudrate;
};
