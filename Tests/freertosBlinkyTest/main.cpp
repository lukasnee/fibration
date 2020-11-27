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
		led.digitalWrite(false);
		vTaskDelay(100);
		led.digitalWrite(true);
		vTaskDelay(1000-100);
		Log::info("led", "message blink  %d !", i++);
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
