#include "input.hpp"
#include <cstring>

Input::Input() : ArgBuffer()
{
    this->reset();
}

void Input::reset()
{
    this->buffer.fill('\0');
    this->charsUsed = 1;
    this->cursorIdx = 0;
}

bool Input::isCursorOnBase()
{
    return (this->cursorIdx == 0);
}

bool Input::isCursorOnEnd()
{
    return (this->charsUsed - this->cursorIdx == 1);
}

bool Input::isEmpty()
{
    return (this->isCursorOnBase() and this->isCursorOnEnd());
}

bool Input::isFull()
{
    return (this->charsUsed >= this->buffer.size());
}

const char &Input::getCharAtCursor()
{
    return this->buffer.at(this->cursorIdx);
}

bool Input::cursorStep()
{
    if (false == this->isFull())
    {
        this->cursorIdx++;
        return true;
    }
    return false;
}

bool Input::cursorStepBack()
{
    if (false == this->isCursorOnBase())
    {
        this->cursorIdx--;
        return true;
    }
    return false;
}

bool Input::deleteCharAtCursor()
{
    if (false == this->isEmpty() && false == this->isCursorOnEnd())
    {
        std::memmove(&this->buffer[this->cursorIdx], &this->buffer[this->cursorIdx + 1], this->charsUsed - (this->cursorIdx + 1));
        this->buffer[--this->charsUsed] = '\0';
        return true;
    }
    return false;
}

const char *Input::getStringAtCursor(std::size_t &lengthOut)
{
    lengthOut = this->charsUsed - this->cursorIdx;
    return &this->buffer[this->cursorIdx];
}

bool Input::backspaceCharAtCursor()
{
    if (false == this->isCursorOnBase())
    {
        std::memmove(&this->buffer[this->cursorIdx - 1], &this->buffer.at(this->cursorIdx), this->charsUsed - this->cursorIdx);
        if (this->cursorStepBack())
        {
            this->buffer[--this->charsUsed] = '\0';
            return true;
        }
    }
    return false;
}

bool Input::insertChar(const char &c)
{
    if (false == this->isFull())
    {
        if (false == this->isCursorOnEnd())
        {
            std::memmove(&this->buffer[this->cursorIdx + 1],
                         &this->buffer.at(this->cursorIdx),
                         this->charsUsed - this->cursorIdx);
        }

        this->buffer[this->cursorIdx] = c;
        this->cursorStep();
        this->buffer[this->charsUsed++] = '\0';
        return true;
    }
    return false;
}
