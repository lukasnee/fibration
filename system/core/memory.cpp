/*
 * Copyright (c) 2025 Lukas Neverauskis https://github.com/lukasnee
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include "ln/logger/logger.h"
#include "ln/shell/CLI.hpp"
#include "FreeRTOS.h"

#include <cstdint>
#include <malloc.h>

LOG_MODULE(mem, LOGGER_LEVEL_NOTSET);

inline void *operator new(size_t size) { return pvPortMalloc(size); }

inline void *operator new[](size_t size) { return pvPortMalloc(size); }

inline void operator delete(void *ptr) { vPortFree(ptr); }

inline void operator delete(void *ptr, unsigned int) { vPortFree(ptr); }

inline void operator delete[](void *ptr) { vPortFree(ptr); }

inline void operator delete[](void *ptr, unsigned int) { vPortFree(ptr); }

extern "C" void *__real_malloc(size_t c);
extern "C" void *__wrap_malloc(int c) {
    auto p = __real_malloc(c);
    // LOG_DEBUG("malloc(%d) = %p", c, p);
    return p;
}

extern "C" void __real_free(void *p);
extern "C" void __wrap_free(void *p) {
    __real_free(p);
    LOG_DEBUG("free(%p)", p);
}

extern "C" void *__real_calloc(size_t n, size_t c);
extern "C" void *__wrap_calloc(int n, int c) {
    auto p = __real_calloc(n, c);
    LOG_DEBUG("calloc(%d, %d) = %p", n, c, p);
    return p;
}

extern "C" void *__real_realloc(void *p, size_t c);
extern "C" void *__wrap_realloc(void *p, int c) {
    auto p2 = __real_realloc(p, c);
    LOG_DEBUG("realloc(%p, %d) = %p", p, c, p2);
    return p2;
}

namespace ln::shell {

Cmd heapsize_cmd{
    Cmd::Cfg{.name = "heapsize", .short_description = "shows current standard heap size usage", .fn = [](Cmd::Ctx ctx) {
                 struct mallinfo info = mallinfo();
                 ctx.cli.printf("Standard heap usage:\n");
                 ctx.cli.printf("  Total non-mmapped bytes (arena):       %d\n", info.arena);
                 ctx.cli.printf("  Number of free chunks (ordblks):       %d\n", info.ordblks);
                 ctx.cli.printf("  Total allocated space (uordblks):      %d\n", info.uordblks);
                 ctx.cli.printf("  Total free space (fordblks):           %d\n", info.fordblks);
                 ctx.cli.printf("  Topmost releasable block (keepcost):   %d\n", info.keepcost);
                 return Err::ok;
             }}};

} // namespace ln::shell

/* relates to configSUPPORT_STATIC_ALLOCATION  */
extern "C" void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer,
                                              configSTACK_DEPTH_TYPE *puxIdleTaskStackSize) {
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];

    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;
    *puxIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

/* relates to configSUPPORT_STATIC_ALLOCATION  */
extern "C" void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                               StackType_t **ppxTimerTaskStackBuffer,
                                               configSTACK_DEPTH_TYPE *puxTimerTaskStackSize) {
    static StaticTask_t xTimerTaskTCB;
    static StackType_t uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH];

    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;
    *puxTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
