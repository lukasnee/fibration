#include "resources.hpp"
#include "system.hpp"

#include <limits>
// NOTE: peripheral instances should get constructer after MCU core and freertos initialization

// TODO: cleanup tim6 mess below, put this logic to lower level source file
// For FreeRTOS TimeGetStats
Tim6 & Periph::getTim6()
{
    // Simple 16-bit value counted-up in 10 KHz
    static Tim6 tim6((SystemCoreClock / (10000)), (std::numeric_limits<std::uint16_t>::max())); 
    return tim6;
}
extern "C" void vConfigureTimerForRunTimeStats()
{
    Periph::getTim6().start();
}

extern "C" uint32_t uint32GetRunTimeCounterValue()
{
    return static_cast<std::uint32_t>(Periph::getTim6().getCounterValue());
}

// For Shell and Logger
Uart2 & Periph::getUart2()
{
    static Uart2 uart2(921600); return uart2;
}

Uart3 & Periph::getUart3()
{
    static Uart3 uart3(921600); return uart3;
}

// For Audio Stream 1
I2s2 & Periph::getI2s2()
{
    return I2s2::getInstance();
}

Adc2 & Periph::getAdc2()
{
    return Adc2::getInstance();
}

// TODO: maybe join port and pin so it would be eg. `Gpio::C3`
// TODO: cleanup, put this logic to lower level source file
Gpio & Periph::getGpio(Gpio::Port port, Gpio::Pin pin)
{
    if (port == Gpio::Port::A)
    {
        if		(pin == Gpio::Pin::pin0)  { static Gpio gpio(port, pin); return gpio; }
        else if (pin == Gpio::Pin::pin1)  { static Gpio gpio(port, pin); return gpio; }
        else if (pin == Gpio::Pin::pin2)  { static Gpio gpio(port, pin); return gpio; }
        else if (pin == Gpio::Pin::pin3)  { static Gpio gpio(port, pin); return gpio; }
        else if (pin == Gpio::Pin::pin4)  { static Gpio gpio(port, pin); return gpio; }
        else if (pin == Gpio::Pin::pin5)  { static Gpio gpio(port, pin); return gpio; }
        else if (pin == Gpio::Pin::pin6)  { static Gpio gpio(port, pin); return gpio; }
        else if (pin == Gpio::Pin::pin7)  { static Gpio gpio(port, pin); return gpio; }
        else if (pin == Gpio::Pin::pin8)  { static Gpio gpio(port, pin); return gpio; }
        else if (pin == Gpio::Pin::pin9)  { static Gpio gpio(port, pin); return gpio; }
        else if (pin == Gpio::Pin::pin10) { static Gpio gpio(port, pin); return gpio; }
        else if (pin == Gpio::Pin::pin11) { static Gpio gpio(port, pin); return gpio; }
        else if (pin == Gpio::Pin::pin12) { static Gpio gpio(port, pin); return gpio; }
        else if (pin == Gpio::Pin::pin13) { static Gpio gpio(port, pin); return gpio; } // SWIO
        else if (pin == Gpio::Pin::pin14) { static Gpio gpio(port, pin); return gpio; } // SWCLK
        else if (pin == Gpio::Pin::pin15) { static Gpio gpio(port, pin); return gpio; }
    }
    else if (port == Gpio::Port::B)
    {
        if		(pin == Gpio::Pin::pin0)  { static Gpio gpio(port, pin); return gpio; }
        else if (pin == Gpio::Pin::pin1)  { static Gpio gpio(port, pin); return gpio; }
        else if (pin == Gpio::Pin::pin2)  { static Gpio gpio(port, pin); return gpio; } // BOOT1
        else if (pin == Gpio::Pin::pin3)  { static Gpio gpio(port, pin); return gpio; }
        else if (pin == Gpio::Pin::pin4)  { static Gpio gpio(port, pin); return gpio; }
        else if (pin == Gpio::Pin::pin5)  { static Gpio gpio(port, pin); return gpio; }
        else if (pin == Gpio::Pin::pin6)  { static Gpio gpio(port, pin); return gpio; }
        else if (pin == Gpio::Pin::pin7)  { static Gpio gpio(port, pin); return gpio; }
        else if (pin == Gpio::Pin::pin8)  { static Gpio gpio(port, pin); return gpio; }
        else if (pin == Gpio::Pin::pin9)  { static Gpio gpio(port, pin); return gpio; }
        else if (pin == Gpio::Pin::pin10) { static Gpio gpio(port, pin); return gpio; }
        else if (pin == Gpio::Pin::pin11) { static Gpio gpio(port, pin); return gpio; }
        else if (pin == Gpio::Pin::pin12) { static Gpio gpio(port, pin); return gpio; }
        else if (pin == Gpio::Pin::pin13) { static Gpio gpio(port, pin); return gpio; }
        else if (pin == Gpio::Pin::pin14) { static Gpio gpio(port, pin); return gpio; }
        else if (pin == Gpio::Pin::pin15) { static Gpio gpio(port, pin); return gpio; }
    }
    else if (port == Gpio::Port::C)
    {
        if 		(pin == Gpio::Pin::pin13) { static Gpio gpio(port, pin); return gpio; } // RobotDyn LED pin (active low)
        else if (pin == Gpio::Pin::pin14) { static Gpio gpio(port, pin); return gpio; }
        else if (pin == Gpio::Pin::pin15) { static Gpio gpio(port, pin); return gpio; }
    }

    FIBSYS_PANIC(); // never returns...
    static Gpio dummy(port, pin); return dummy;
}