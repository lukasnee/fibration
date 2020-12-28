#include "dmaQueue.hpp"

#define QUEUE_MAX 0x40

DmaQueue::DmaQueue(uint32_t priority) : 
    Thread{"dmaQueue", 0x100, priority}, //todo priority move out
    txCpltCount(0)
{
    Thread::Start();
}

bool DmaQueue::txPush(const std::uint8_t * ptr, std::uint16_t size)
{
    bool retval = false;

    if(txQueue.size() < QUEUE_MAX) // some limit
    {
        DmaQueue::DmaSession blob = { .ptr = const_cast<std::uint8_t*>(ptr), .size = size, .dmaCallbacks = *this };
        txQueue.push(blob);
        Thread::Resume(); // handle immediately
        retval = true;
    }

    return retval;
}

void DmaQueue::dmaTxCpltCallback()
{
    txCpltCount++;
    Thread::ResumeFromISR();
}

void DmaQueue::Run() // task code
{
    while(true)
    {
        txHandle();

        Thread::Suspend();
    }
}

bool DmaQueue::txHandle()
{
    bool retval = false;

    // free completed session from queue front
    if (txCpltCount)
    {
        delete[] txQueue.front().ptr;
        txQueue.pop();
        txCpltCount--;
        retval = true;
    }

    // tx next if queue's not empty
    retval |= txNext();

    return retval;
}

bool DmaQueue::txNext()
{
    bool retval = false;

    if(txQueue.empty() == false)
    {
        if (transmitDma(txQueue.front().ptr, 
                        txQueue.front().size, 
                        txQueue.front().dmaCallbacks))
        {
            retval = true;
        }
    }

    return retval;
}

DmaQueue::~DmaQueue()
{
}
