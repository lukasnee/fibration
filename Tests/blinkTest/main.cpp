#include <cstdint>

#include "FreeRTOS.h"
#include "task.h"

#include "system.hpp"
#include "gpio.hpp"

void vMainTask(void * pvParams)
{
	Gpio & onBoardLed = Periph::getGpio(Gpio::Port::A, Gpio::Pin::P13);
	onBoardLed.init(false, Gpio::Mode::Output_PP);
	int i = 0;
 
	for(;;)
	{	
		onBoardLed.write(false);
		vTaskDelay(100);
		onBoardLed.write(true);
		vTaskDelay(1000-100);
		Log::info("led", "message blink  %d !", i++);
	}
	vTaskDelete(NULL);
}
 