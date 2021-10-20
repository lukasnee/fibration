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

bool Input::setCursor(size_t index)
{
    bool result = false;

    if (index < this->buffer.size())
    {
        this->cursorIdx = index;
        result = true;
    }

    return result;
}

bool Input::cursorStepRight()
{
    bool result = false;

    if (!this->isFull())
    {
        this->cursorIdx++;
        result = true;
    }

    return result;
}

bool Input::cursorStepLeft()
{
    bool result = false;

    if (!this->isCursorOnBase())
    {
        this->cursorIdx--;
        result = true;
    }

    return result;
}

bool Input::deleteCharAtCursor()
{
    bool result = false;

    if (!this->isEmpty() && !this->isCursorOnEnd())
    {
        std::memmove(&this->buffer[this->cursorIdx], &this->buffer[this->cursorIdx + 1], this->charsUsed - (this->cursorIdx + 1));
        this->buffer[--this->charsUsed] = '\0';
        result = true;
    }

    return result;
}

const char *Input::getBufferAtCursor(std::size_t &lengthOut)
{
    lengthOut = this->charsUsed - this->cursorIdx;
    return &this->buffer[this->cursorIdx];
}

const char *Input::getBufferAtBase()
{
    return &this->buffer[0];
}

bool Input::backspaceCharAtCursor()
{
    bool result = false;

    if (false == this->isCursorOnBase())
    {
        std::memmove(&this->buffer[this->cursorIdx - 1], &this->buffer.at(this->cursorIdx), this->charsUsed - this->cursorIdx);
        if (this->cursorStepLeft())
        {
            this->buffer[--this->charsUsed] = '\0';
            result = true;
        }
    }

    return result;
}

bool Input::insertChar(const char &c)
{
    bool result = false;

    if (!this->isFull())
    {
        if (!this->isCursorOnEnd())
        {
            std::memmove(&this->buffer[this->cursorIdx + 1],
                         &this->buffer.at(this->cursorIdx),
                         this->charsUsed - this->cursorIdx);
        }

        this->buffer[this->cursorIdx] = c;
        this->cursorStepRight();
        this->buffer[this->charsUsed++] = '\0';
        result = true;
    }
    return result;
}
