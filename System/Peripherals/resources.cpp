#include "resources.hpp"
#include "system.hpp"

#include <limits>
// NOTE: peripheral instances should get constructer after MCU core and freertos initialization

// TODO: cleanup tim6 mess below, put this logic to lower level source file
// For FreeRTOS TimeGetStats
Tim6 &Periph::getTim6()
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
Uart2 &Periph::getUart2()
{
    static Uart2 uart2(921600);
    return uart2;
}

Uart3 &Periph::getUart3()
{
    static Uart3 uart3(921600);
    return uart3;
}

// For Audio Stream 1
I2s2 &Periph::getI2s2()
{
    return I2s2::getInstance();
}

Adc2 &Periph::getAdc2()
{
    return Adc2::getInstance();
}
