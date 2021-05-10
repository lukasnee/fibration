#include "stringContainer.hpp"

StringContainer::StringContainer() {}

char *StringContainer::getBase() { return this->pBase; }
char *StringContainer::getHead() { return this->pHead; }
int StringContainer::getSize() { return this->size; }
int StringContainer::getCharsUsed() { return (this->getHead() - this->getBase()); }
int StringContainer::getCharsLeft() { return (this->getSize() - this->getCharsUsed()); }

bool StringContainer::allocate(const int &maxStringLength)
{
    bool result = false;

    this->pBase = this->pHead = new char[maxStringLength + sizeof('\0')];
    if (this->pBase)
    {
        this->size = maxStringLength;
        result = true;
    }

    return result;
}

void StringContainer::free()
{
    delete[] this->pBase;
    this->pBase = nullptr;
    this->pHead = nullptr;
    this->size = 0;
}

void StringContainer::reset()
{
    this->pHead = this->getBase();
}

bool StringContainer::step(const int &stepInNumOfChars)
{
    bool result = false;

    if (stepInNumOfChars <= this->getCharsLeft())
    {
        this->pHead += stepInNumOfChars;

        result = true;
    }

    return result;
}
