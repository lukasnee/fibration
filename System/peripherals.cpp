#include "peripherals.hpp"
#include "system.hpp"

Uart & Periph::getUart(std::uint8_t no)
{
	if		(no == 1) { static Uart uart1(1, 115200); return uart1; }
	else if (no == 2) { /* ... */ }

	FibSys::error(); // never returns...
	static Uart dummy(0, 0); return dummy;
}

Gpio & Periph::getGpio(Gpio::Port port, Gpio::Pin pin)
{
	if (port == Gpio::Port::A)
	{ 
		if		(pin == Gpio::Pin::P0)  { static Gpio gpio(port, pin); return gpio; }
		else if (pin == Gpio::Pin::P1)  { static Gpio gpio(port, pin); return gpio; }
		else if (pin == Gpio::Pin::P2)  { static Gpio gpio(port, pin); return gpio; }
		else if (pin == Gpio::Pin::P3)  { static Gpio gpio(port, pin); return gpio; }
		else if (pin == Gpio::Pin::P4)  { static Gpio gpio(port, pin); return gpio; }
		else if (pin == Gpio::Pin::P5)  { static Gpio gpio(port, pin); return gpio; }
		else if (pin == Gpio::Pin::P6)  { static Gpio gpio(port, pin); return gpio; }
		else if (pin == Gpio::Pin::P7)  { static Gpio gpio(port, pin); return gpio; }
		else if (pin == Gpio::Pin::P8)  { static Gpio gpio(port, pin); return gpio; }
		else if (pin == Gpio::Pin::P9)  { static Gpio gpio(port, pin); return gpio; }
		else if (pin == Gpio::Pin::P10) { static Gpio gpio(port, pin); return gpio; }
		else if (pin == Gpio::Pin::P11) { static Gpio gpio(port, pin); return gpio; }
		else if (pin == Gpio::Pin::P12) { static Gpio gpio(port, pin); return gpio; }
		else if (pin == Gpio::Pin::P13) { static Gpio gpio(port, pin); return gpio; } // SWIO
		else if (pin == Gpio::Pin::P14) { static Gpio gpio(port, pin); return gpio; } // SWCLK
		else if (pin == Gpio::Pin::P15) { static Gpio gpio(port, pin); return gpio; }
	}
	if (port == Gpio::Port::B)
	{ 
		if		(pin == Gpio::Pin::P0)  { static Gpio gpio(port, pin); return gpio; }
		else if (pin == Gpio::Pin::P1)  { static Gpio gpio(port, pin); return gpio; }
		else if (pin == Gpio::Pin::P2)  { static Gpio gpio(port, pin); return gpio; } // BOOT1
		else if (pin == Gpio::Pin::P3)  { static Gpio gpio(port, pin); return gpio; }
		else if (pin == Gpio::Pin::P4)  { static Gpio gpio(port, pin); return gpio; }
		else if (pin == Gpio::Pin::P5)  { static Gpio gpio(port, pin); return gpio; }
		else if (pin == Gpio::Pin::P6)  { static Gpio gpio(port, pin); return gpio; }
		else if (pin == Gpio::Pin::P7)  { static Gpio gpio(port, pin); return gpio; }
		else if (pin == Gpio::Pin::P8)  { static Gpio gpio(port, pin); return gpio; }
		else if (pin == Gpio::Pin::P9)  { static Gpio gpio(port, pin); return gpio; }
		else if (pin == Gpio::Pin::P10) { static Gpio gpio(port, pin); return gpio; }
		else if (pin == Gpio::Pin::P11) { static Gpio gpio(port, pin); return gpio; }
		else if (pin == Gpio::Pin::P12) { static Gpio gpio(port, pin); return gpio; }
		else if (pin == Gpio::Pin::P13) { static Gpio gpio(port, pin); return gpio; }
		else if (pin == Gpio::Pin::P14) { static Gpio gpio(port, pin); return gpio; }
		else if (pin == Gpio::Pin::P15) { static Gpio gpio(port, pin); return gpio; }
	}
	if (port == Gpio::Port::C)
	{ 
		if 		(pin == Gpio::Pin::P13) { static Gpio gpio(port, pin); return gpio; } // RobotDyn LED pin (active low)
		else if (pin == Gpio::Pin::P14) { static Gpio gpio(port, pin); return gpio; }
		else if (pin == Gpio::Pin::P15) { static Gpio gpio(port, pin); return gpio; }
	}

	FibSys::error(); // never returns...
	static Gpio dummy(port, pin); return dummy;
}