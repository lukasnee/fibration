#pragma once

#include "dmaQueue.hpp"

class  Uart1 : public DmaQueue {
public:
	Uart1(uint32_t baudrate);

	bool transmitDma(std::uint8_t *pData, std::uint16_t size, DmaCallbacks &dmaCallbacks);
	bool receiveDma(std::uint8_t *pData, std::uint16_t size, DmaCallbacks &dmaCallbacks);

    void operator=(Uart1 const&)  = delete;
	Uart1(Uart1 const&) = delete;
	~Uart1();

	DmaCallbacks * dmaCallbacks;

protected:
private:
	uint32_t baudrate;
};
