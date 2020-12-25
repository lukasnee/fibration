#include <cstdint>

#include "FreeRTOS.h"
#include "task.h"

#include "system.hpp"
#include "gpio.hpp"

void mainTask(void * pvParams)
{
	Gpio & onBoardLed = Periph::getGpio(Gpio::Port::C, Gpio::Pin::P13);
	onBoardLed.init(false, Gpio::Mode::Output_PP);
	int i = 0;
 
	while(true)
	{	
		onBoardLed.write(false);
		vTaskDelay(500);
		onBoardLed.write(true);
		vTaskDelay(500);
		Log::info("led", "message blink  %d !", i++);
	}
}
