#pragma once

#include "argBuffer.hpp"

#include <cstdint>
#include <array>

class Input : public ArgBuffer
{
public:
    Input();
    ~Input() = default;

    void reset();

    bool isFull();
    bool isEmpty();

    const char &getCharAtCursor();
    const char *getBufferAtCursor(std::size_t &lengthOut);
    const char *getBufferAtBase();

    bool isCursorOnBase();
    bool isCursorOnEnd();

    bool cursorStep();
    bool cursorStepBack();

    bool deleteCharAtCursor();
    bool backspaceCharAtCursor();
    bool insertChar(const char &c);

private:
    std::size_t cursorIdx = 0;
    std::size_t charsUsed = 1;
};
