#pragma once 

#include "tim6.hpp" 
#include "uart1.hpp" 
#include "uart2.hpp" 
#include "gpio.hpp"

class Periph 
{
public:
	static Tim6 & getTim6();
	static Uart1 & getUart1();
	static Uart2 & getUart2();
	static Gpio & getGpio(Gpio::Port port, Gpio::Pin pin);
};
