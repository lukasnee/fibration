#include <cstdint>

#include "FreeRTOS.h"
#include "task.h"

#include "system.hpp"
#include "gpio.hpp"

void vMainTask(void * pvParams)
{
	Gpio & onBoardLed = Periph::getGpio(Gpio::Port::C, Gpio::Pin::P13);
	onBoardLed.init(false, Gpio::Mode::Output_PP);
	int i = 0;
 
	for(;;)
	{	
		onBoardLed.write(false);
		vTaskDelay(500);
		onBoardLed.write(true);
		vTaskDelay(500);
		Log::info("led", "message blink  %d !", i++);
	}
	vTaskDelete(NULL);
}
