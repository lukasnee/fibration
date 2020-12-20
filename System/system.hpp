#pragma once

#include <cstdint>

#include "FreeRTOS.h"
#include "task.h"

#include "peripheral.hpp"
#include "log.hpp"

class System // todo make singleton
{
public:
	static void run();
	static void stop();
	static uint32_t getTick();
	static void error();
private:
	static void hardwareInit();
	static void createTask(uint32_t priority);

	static TaskHandle_t hMainTask;
};

void vMainTask(void *); // main application task
