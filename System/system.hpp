#pragma once

#include <cstdint>

#include "FreeRTOS.h"
#include "task.h"

#include "peripherals.hpp"
#include "log.hpp"

class FibSys // todo make singleton
{
public:
    static void run();
    static void stop();
    static std::uint32_t getTick();
    static void taskDelay(std::uint32_t ms);
    static void error();
    
private:
    static void initPlatform();
    static void initTasks(std::uint32_t priority);

    static void vSystemTask(void * pvParams);
    static void vMainTask(void * pvParams);

    static TaskHandle_t hSystemTask;
    static TaskHandle_t hMainTask;

    static size_t initialFreeHeapSize;
    static size_t freeHeapSize;
};

void mainTask(void *); // application entry point
