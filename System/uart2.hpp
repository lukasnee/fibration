#pragma once

#include "stream.hpp"
#include "uartInterface.hpp"
#include <cstdint>

class  Uart2 : public UartInterface {
public:
    Uart2(std::uint32_t baudrate = 115200);
    ~Uart2();
    bool init() override;
    bool deinit() override;
    bool tx(std::uint8_t *pData, std::uint16_t size) override;
    bool rx(std::uint8_t *pData, std::uint16_t size) override;

    void operator=(Uart2 const&)  = delete;
    Uart2(Uart2 const&) = delete;
private:
    std::uint32_t baudrate;
};
