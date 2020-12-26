#pragma once 

#include "uart1.hpp" 
#include "gpio.hpp"

class Periph 
{
public:
	static Uart1 & getUart1();
	static Gpio & getGpio(Gpio::Port port, Gpio::Pin pin);
};
