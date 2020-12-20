#pragma once

#include <cstdint>

#include "FreeRTOS.h"
#include "task.h"

#include "peripherals.hpp"
#include "log.hpp"

class System // todo make singleton
{
public:
	static void run();
	static void stop();
	static std::uint32_t getTick();
	static void error();
	
private:
	static void hardwareInit();
	static void createTask(std::uint32_t priority);

	static TaskHandle_t hMainTask;
};

void vMainTask(void *); // main application task
