#include <cstdint>

#include "FreeRTOS.h"
#include "thread.hpp"
#include "timer.hpp"
#include "ticks.hpp"

#include "system.hpp"
#include "peripherals.hpp"
#include "log.hpp"

int main() 
{
    FibSys::start();
}

class BlinkTestApp : public cpp_freertos::Thread
{
public:
    BlinkTestApp(std::uint16_t stackDepth) : 
        Thread("blinkTestApp", 0x200, FibSys::getAppMaxPriority())
    {
        if(!Start())
        {
            FibSys::panic();
        }
    };
private:
    virtual void Run() override;
};

BlinkTestApp blinkTestApp(0x200);

void BlinkTestApp::Run()
{
    Gpio &onBoardLed = Periph::getGpio(Gpio::Port::C, Gpio::Pin::pin13);
    Gpio &rotaryButton = Periph::getGpio(Gpio::Port::C, Gpio::Pin::pin14);
    
    onBoardLed.init(Gpio::Mode::outputPushPull, Gpio::PinState::low);
    rotaryButton.init(Gpio::Mode::input, Gpio::Pull::up);

    int i = 0;

    while (true)
    {
        rotaryButton.read() ? Log::colorEnable() : Log::colorDisable(); // todo config over uart

        onBoardLed.write(Gpio::PinState::low);
        Delay(cpp_freertos::Ticks::MsToTicks(500));
        Log::info("mainTask", "blink %d !", i++);
        onBoardLed.write(Gpio::PinState::high);
        Delay(cpp_freertos::Ticks::MsToTicks(500));

        for (int y = 0; y < i; y++)
        {
            Log::trace("mainTask", "%.*s", y, "=================================================================================");
        }
        i++;
    }

    while (true) { Delay(1); }
}