#pragma once

#include "stdint.h"

#define ANSI_COLOR_BLACK "\e[30m"
#define ANSI_COLOR_RED "\e[31m"
#define ANSI_COLOR_GREEN "\e[32m"
#define ANSI_COLOR_YELLOW "\e[33m"
#define ANSI_COLOR_BLUE "\e[34m"
#define ANSI_COLOR_MAGENTA "\e[35m"
#define ANSI_COLOR_CYAN "\e[36m"
#define ANSI_COLOR_WHITE "\e[37m"
#define ANSI_COLOR_DEFAULT "\e[39m"
#define ANSI_COLOR_RESET "\e[0m"

typedef struct __attribute__((packed)) ExceptionStackFrame
{
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    uint32_t lr;
    uint32_t return_address;
    uint32_t xpsr;
} ExceptionStackFrame;

void fibsys_panic(const char *strFile, uint32_t line);
void fibsys_hardfault(ExceptionStackFrame *pExceptionStackFrame, char stackPointerInitial);

#define FIBSYS_PANIC() fibsys_panic(__FILE__, __LINE__)

#define FIBSYS_HARDFAULT(pExceptionStackFrame, stackPointerInitial) fibsys_hardfault(pExceptionStackFrame, stackPointerInitial)