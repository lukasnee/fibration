#pragma once

#include <cstdint>

class  Uart2 {
public:
    Uart2(std::uint32_t baudrate);

    void putc(char c);
    char getc(void);

    void operator=(Uart2 const&)  = delete;
    Uart2(Uart2 const&) = delete;
    ~Uart2();
protected:

private:
    std::uint32_t baudrate;
};
