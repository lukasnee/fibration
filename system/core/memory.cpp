// global C++ style dynamic memory allocation override with thread-safe FreeRTOS methods
#include "FreeRTOS.h"
#include <cstdint>

inline void *operator new(size_t size)
{
    return pvPortMalloc(size);
}

inline void *operator new[](size_t size)
{
    return pvPortMalloc(size);
}

inline void operator delete(void *ptr)
{
    vPortFree(ptr);
}

inline void operator delete(void *ptr, unsigned int)
{
    vPortFree(ptr);
}

inline void operator delete[](void *ptr)
{
    vPortFree(ptr);
}

inline void operator delete[](void *ptr, unsigned int)
{
    vPortFree(ptr);
}

/* relates to configSUPPORT_STATIC_ALLOCATION  */
extern "C" void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer,
                                              uint32_t *pulIdleTaskStackSize)
{
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];

    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

/* relates to configSUPPORT_STATIC_ALLOCATION  */
extern "C" void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                               StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize)
{
    static StaticTask_t xTimerTaskTCB;
    static StackType_t uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH];

    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
