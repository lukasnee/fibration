#pragma once 

#include "Peripherals/tim6.hpp" 
#include "Peripherals/uart1.hpp" 
#include "Peripherals/uart2.hpp" 
#include "Peripherals/i2s2.hpp" 
#include "Peripherals/gpio.hpp"

class Periph 
{
public:
	static Tim6 & getTim6();
	static Uart1 & getUart1();
	static Uart2 & getUart2();
	static I2s2 & getI2s2();
	static Gpio & getGpio(Gpio::Port port, Gpio::Pin pin);
};
