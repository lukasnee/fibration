#pragma once 

#include "Peripherals/tim6.hpp" 
#include "Peripherals/uart2.hpp" 
#include "Peripherals/uart3.hpp" 
#include "Peripherals/i2s2.hpp" 
#include "Peripherals/gpio.hpp"
#include "Peripherals/adc2.hpp"

class Periph 
{
public:
	static Tim6 & getTim6();
	static Uart2 & getUart2();
	static Uart3 & getUart3();
	static I2s2 & getI2s2();
	static Adc2 & getAdc2();
	static Gpio & getGpio(Gpio::Port port, Gpio::Pin pin);
};
