#include <cstdint>

#include "FreeRTOS.h"
#include "task.h"

#include "system.hpp"
#include "gpio.hpp"

void mainTask(void *pvParams)
{
    Gpio &onBoardLed = Periph::getGpio(Gpio::Port::C, Gpio::Pin::pin13);
    Gpio &rotaryButton = Periph::getGpio(Gpio::Port::C, Gpio::Pin::pin14);
    
    onBoardLed.init(Gpio::Mode::outputPushPull, Gpio::PinState::low);
    rotaryButton.init(Gpio::Mode::input, Gpio::Pull::up);

    int i = 0;

    Log::error("mainTask", "blink %d !", i++);
    Log::fatal("mainTask", "blink %d !", i++);
    Log::info("mainTask", "blink %d !", i++);
    Log::system("mainTask", "blink %d !", i++);
    Log::trace("mainTask", "blink %d !", i++);
    Log::warning("mainTask", "blink %d !", i++);


    while (true)
    {
        rotaryButton.read() ? Log::colorEnable() : Log::colorDisable();

        onBoardLed.write(Gpio::PinState::low);
        Log::info("mainTask", "blink %d !", i);
        for (int y = 0; y < i; y++)
        {
            Log::trace("mainTask", "%.*s", y, "=================================================================================");
        }
        i++;
        vTaskDelay(500);
        onBoardLed.write(Gpio::PinState::high);
        vTaskDelay(500);
    }
}
