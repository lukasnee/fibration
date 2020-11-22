#include <cstdint>

#include "FreeRTOS.h"
#include "task.h"

#include "system.hpp"
#include "gpio.hpp"

void vBlinkyTask(void * pvParams)
{
	
	Hardware::GPIO led(Hardware::GPIO::PIN_LED);
	int i = 0;
	while(true)
	{
		
		led.digitalWrite(true);
		vTaskDelay(1000-100);
		led.digitalWrite(false);
		vTaskDelay(100);

		if(!(i%3)) Log::message("led", "message blink  %d !", i);
		if(!(i%5)) Log::warning("led", "warning blink  %d !", i);
		if(!(i%7)) Log::error("led", "error blink %d !", i);
		i++;
	}
}

int main()
{  
	System::init();

	BaseType_t xReturned;
	TaskHandle_t xHandle = NULL;
	xReturned = xTaskCreate(vBlinkyTask, "blinky", 0x200, NULL, 1, &xHandle);
		
	if(xReturned == pdPASS)
    { 
		vTaskStartScheduler();
	}
	else
	{
		vTaskDelete(xHandle);
	}
	while(true); // should never reach
}
