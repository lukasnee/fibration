#pragma once

#include <array>
#include <queue>

class DmaCallbacks {
public:
	// void (*txComplete)(void);
	// void (*rxComplete)(void);

	virtual void txComplete() = 0;
	virtual void rxComplete() = 0;

	// static bool tryToCall(void (*callback)(void));

	DmaCallbacks(){}

	// DmaCallbacks() : 
	// 	txComplete(nullptr), 
	// 	rxComplete(nullptr) 
	// {
	// }
};

// todo make template for buffer size
// todo make array type not char but more general uint8_t
// todo make list of staticBuffers

class DmaQueue : public DmaCallbacks {
public:	
	struct Blob { std::uint8_t * ptr; std::size_t size; };
	
// SETUP:

	// give it interface - can be single or bidirectional
	virtual bool transmitDma(
		DmaCallbacks &dmaCb, std::uint8_t *pData, std::uint16_t size) = 0;

	virtual bool receiveDma(
		DmaCallbacks &dmaCb, std::uint8_t *pData, std::uint16_t size) = 0;
	// call this when stream finishes (expected to be interrupt based)

	void txComplete() override;
	void rxComplete() override;

// USAGE:

	// stream loading interface, use custom speed-optimized alloc()
	bool alloc(std::size_t size, std::uint8_t ** ppAlloc, std::size_t * allocSize);
	// when data fully loaded...
	bool isQueueFull();
	bool queuePush(const std::uint8_t * ptr, std::size_t size);
	// or 
	void free(std::uint8_t * ptr);
	// if something bad happed ;)

	DmaQueue();
	~DmaQueue();
protected:
private:	

	std::deque<Blob> deque;
	bool fsbInUse;
	std::array<std::uint8_t, 0x80> fsb;

	bool queueStreamNext();
	bool queuePop();
};
