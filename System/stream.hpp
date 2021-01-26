#pragma once

class Stream {
public:
    virtual void putc(char c) = 0;
    virtual char getc(void) = 0;
};