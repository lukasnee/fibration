#pragma once

#include <cstdint>

class StringContainer
{
public:
    StringContainer();
    char *getBase();
    char *getHead();
    int getSize();
    int getCharsUsed();
    int getCharsLeft();
    bool allocate(const int &maxStringLength);
    void free();
    void reset();
    bool step(const int &stepInNumOfChars);

private:
    char *pBase = nullptr;
    char *pHead = nullptr;
    std::size_t size = 0;
};
