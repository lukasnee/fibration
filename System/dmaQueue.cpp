#include "dmaQueue.hpp"

#include "FreeRTOS.h"

#define QUEUE_MAX 0x20

// todo rename stream to object name 
// low level (hardware DMA) interface
// should be signaled on DMA stream completion interrupt callback

void DmaQueue::rxComplete()
{
	// todo
}

/* NOTE ! every alloc() must follow according queueStreamNext() or free() */
bool DmaQueue::alloc(std::size_t size, std::uint8_t ** pPtr, std::size_t * pAllocSize)
{
	bool retval = false;

	std::uint8_t * allocPtr = NULL;
	std::size_t allocSize = 0;

	if(deque.size() < QUEUE_MAX - 1)
	{
		// allocate fast static buffer if available
		if(fsbInUse == false && size <= fsb.size())
		{
			allocPtr = fsb.data();
			allocSize = fsb.size(); 
			fsbInUse = true;
		}
		else // elsewise allocate mem ory from heap...
		{
			// NOTE. FreeRTOS vPortFree fails from ISR // todo ivestigate
			allocPtr = new std::uint8_t[size == 0 ? fsb.size() : size];	
			allocSize = (size == 0 ? fsb.size() : size);
		}
	}

	retval = (allocPtr != NULL);

	*pPtr = allocPtr;
	*pAllocSize = allocSize;

	return retval;
}

bool DmaQueue::queuePush(const std::uint8_t * ptr, std::size_t size)
{
	if(deque.size() < QUEUE_MAX)
	{
		DmaQueue::Blob blob = { .ptr = const_cast<std::uint8_t*>(ptr), .size = size };
		deque.push_back(blob);
	}
	queueStreamNext(); // try to stream it immediately
}

bool DmaQueue::queueStreamNext()
{
	bool retval = false;

	if(deque.empty() == false)
	{
		if (transmitDma(*this, deque.front().ptr, deque.front().size) == false)
		{
			retval = true;
		}
	}

	return retval;
}

// pop completed blob and try to stream next one
void DmaQueue::txComplete()
{
	DmaQueue::queuePop(); 
	queueStreamNext();
}

bool DmaQueue::queuePop()
{
	bool retval = false;

	if(deque.empty() == false)
	{	
		DmaQueue::free(deque.front().ptr);
		deque.pop_front();
		retval = true;
	}

	return retval;
}

// todo maybe change to Blob passing as arg
void DmaQueue::free(std::uint8_t * ptr)
{
	if(ptr == fsb.data())
	{
		//memset(fsb.data(), 0 , fsb.size()) // unnecessary
		fsbInUse = false;
	}
	else
	{
		// NOTE. FreeRTOS vPortFree fails from ISR // todo ivestigate
		delete [] ptr;
	}
}

DmaQueue::DmaQueue() :  
	fsbInUse(false) 
{
}
 
DmaQueue::~DmaQueue()
{
}
