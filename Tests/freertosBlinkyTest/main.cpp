#include <cstdint>

#include "FreeRTOS.h"
#include "task.h"

#include "system.hpp"
#include "gpio.hpp"

void vMainTask(void * pvParams)
{
	Hardware::GPIO led(Hardware::GPIO::PIN_LED);
	int i = 0;

	for(;;)
	{	
		led.digitalWrite(false);
		vTaskDelay(100);
		led.digitalWrite(true);
		vTaskDelay(1000-100);
		Log::info("led", "message blink  %d !", i++);
	}
}
