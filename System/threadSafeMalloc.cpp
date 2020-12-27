// global C++ style dynamic memory allocation override with thread-safe FreeRTOS methods
#include <cstdint>
#include "FreeRTOS.h"

void * operator new(size_t size)
{
    return pvPortMalloc(size);
}

void * operator new[](size_t size)
{
    return pvPortMalloc(size);
}

void operator delete(void * ptr)
{
    vPortFree(ptr);
}

void operator delete[](void * ptr)
{
    vPortFree(ptr);
}
