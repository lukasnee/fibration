#include "peripherals.hpp"
#include "system.hpp"

#include <limits>
// NOTE: peripheral instances should get constructer after MCU core and freertos initialization

// Gets used for FreeRTOS TimeGetStats
Tim6 & Periph::getTim6()
{
    static Tim6 tim6((72000000/ (10000)), (std::numeric_limits<std::uint16_t>::max())); return tim6;
}
extern "C" void vConfigureTimerForRunTimeStats()
{
    Periph::getTim6().start(); // constructs (inits) and starts.
}
extern "C" uint32_t uint32GetRunTimeCounterValue()
{
    return Periph::getTim6().getCounterValue();
}

Uart1 & Periph::getUart1()
{
    static Uart1 uart1(10, 115200); return uart1; // todo figure out priority 
}

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

    FibSys::panic(); // never returns...
    static Gpio dummy(port, pin); return dummy;
}