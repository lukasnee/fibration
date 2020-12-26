#pragma once

#include <cstdint>

#include "dmaQueue.hpp"

class  Uart1 : public DmaQueue {
// todo session ?
public:

	Uart1(uint32_t baudrate);

	bool transmitDma(DmaCallbacks &dmaCb, std::uint8_t *pData, std::uint16_t size);
	bool receiveDma(DmaCallbacks &dmaCb, std::uint8_t *pData, std::uint16_t size);

    void operator=(Uart1 const&)  = delete;
	Uart1(Uart1 const&) = delete;
	~Uart1();

	DmaCallbacks * dmaCallbacks;

protected:
private:
	uint32_t baudrate;
};
