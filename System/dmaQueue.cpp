#include "dmaQueue.hpp"

#include "FreeRTOS.h"

#define QUEUE_MAX 0x20

bool DmaQueue::outQueuePush(const std::uint8_t * ptr, std::size_t size)
{
	bool retval = false;

	if(outDeque.size() < QUEUE_MAX)
	{
		DmaQueue::Blob blob = { .ptr = const_cast<std::uint8_t*>(ptr), .size = size };
		outDeque.push_back(blob);
		retval = true;
	}
	outQueueStreamNext(); // try to stream it immediately

	return retval;
}

bool DmaQueue::outQueueStreamNext()
{
	bool retval = false;

	if(outDeque.empty() == false)
	{
		if (transmitDma(outDeque.front().ptr, outDeque.front().size, *this) == false)
		{
			retval = true;
		}
	}

	return retval;
}

// pop completed blob and try to stream next one
void DmaQueue::dmaTxCpltCallback()
{
	DmaQueue::outQueuePop(); 
	outQueueStreamNext();
}

bool DmaQueue::outQueuePop()
{
	bool retval = false;

	if(outDeque.empty() == false)
	{	
		delete [] outDeque.front().ptr;
		outDeque.pop_front();
		retval = true;
	}

	return retval;
}

DmaQueue::DmaQueue() 
{
}
 
DmaQueue::~DmaQueue()
{
}
