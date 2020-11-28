#pragma once

#include "log.hpp"

class System
{
public:
	static void start();
	static uint32_t getTick();
private:
	static void createSystemTasks();
};