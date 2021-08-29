#pragma once

#include "ioDataIF.hpp"
#include <cstdint>

class Uart3 : public IODataIF
{
public:
    Uart3(std::uint32_t baudrate = 115200);
    ~Uart3();

protected:
    bool initUnsafe() override;
    bool deinitUnsafe() override;
    bool txUnsafe(const std::uint8_t *pData, std::size_t size) override;
    bool rxUnsafe(std::uint8_t *pData, std::size_t size) override;

    void operator=(Uart3 const &) = delete;
    Uart3(Uart3 const &) = delete;

private:
    std::uint32_t baudrate;
};
