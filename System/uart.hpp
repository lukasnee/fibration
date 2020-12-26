#pragma once

#include <cstdint>

#include "dmaQueue.hpp"

class  Uart : public DmaQueue {
public:
	bool transmitDma(DmaCallbacks &dmaCb, std::uint8_t *pData, std::uint16_t size);
	bool receiveDma(DmaCallbacks &dmaCb, std::uint8_t *pData, std::uint16_t size);

	// static struct Session : public DmaCallbacks
	// {
	// 	DmaCallbacks & dmaQueueCallbacks;
	// 	Session() : DmaQueue(session) {}	

	// 	void txComplete() override;
	// 	void rxComplete() override;
	// } session;

	DmaCallbacks * dmaCallbacks;

    void operator=(Uart const&)  = delete;
	Uart(Uart const&) = delete;
	
	Uart(std::uint8_t no, uint32_t baudrate);
	~Uart();

protected:
private:
	uint8_t no; // uart hardware peripheral number
	uint32_t baudrate;
// todo session ?
};
