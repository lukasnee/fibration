#pragma once

#include <cstdint>
#include <array>
#include <queue>

class DmaCallbacks 
{
public:
	DmaCallbacks(){};
	virtual void dmaTxCpltCallback() {};
	virtual void dmaTxHalfCpltCallback() {};
	virtual void dmaRxCpltCallback() {};
	virtual void dmaRxHalfCpltCallback() {};
	virtual void dmaErrorCallback() {}; 
	virtual void dmaAbortCpltCallback() {};
	virtual void dmaAbortTransmitCpltCallback() {};
	virtual void dmaAbortReceiveCpltCallback() {};
};

// todo make template for buffer size
// todo make array type not char but more general uint8_t
// todo make list of staticBuffers

class DmaQueue : public DmaCallbacks 
{
public:	
	struct Blob { std::uint8_t * ptr; std::size_t size; };

	virtual bool transmitDma(std::uint8_t *pData, std::uint16_t size, DmaCallbacks &dmaCallbacks) = 0;
	virtual bool receiveDma(std::uint8_t *pData, std::uint16_t size, DmaCallbacks &dmaCallbacks) = 0;

	bool pushTx(const std::uint8_t * ptr, std::size_t size);

	void dmaTxCpltCallback() override;
	// void dmaTxHalfCpltCallback() override;
	// void dmaRxCpltCallback() override;
	// void dmaRxHalfCpltCallback() override;
	// void dmaErrorCallback() override;
	// void dmaAbortCpltCallback() override;
	// void dmaAbortTransmitCpltCallback() override;
	// void dmaAbortReceiveCpltCallback() override;

protected:
	DmaQueue();
	~DmaQueue();
private:	
	std::deque<Blob> outDeque;
	bool outQueueStreamNext();
	bool outQueuePop();
};
