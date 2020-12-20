#pragma once 

#include "uart.hpp" 
#include "gpio.hpp"

class Periph 
{
public:
	static Uart & getUart(std::uint8_t no);
	static Gpio & getGpio(Gpio::Port port, Gpio::Pin pin);
};
