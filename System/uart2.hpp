#pragma once

#include "stream.hpp"
#include <cstdint>

class  Uart2 : public Stream {
public:
    Uart2(std::uint32_t baudrate);

    void putc(char c) override;
    char getc(void) override;

    void operator=(Uart2 const&)  = delete;
    Uart2(Uart2 const&) = delete;
    ~Uart2();
protected:

private:
    std::uint32_t baudrate;
};
