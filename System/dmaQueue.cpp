#include "dmaQueue.hpp"

#define QUEUE_MAX 0x20

DmaQueue::DmaQueue(uint32_t priority) : 
    Thread{"dmaQueue", 0x100, priority}, //todo priority move out
    txCpltCount(0)
{
    Thread::Start();
}

bool DmaQueue::txPush(const std::uint8_t * ptr, std::uint16_t size)
{
    bool retval = false;

    if(txSessionQueue.size() < QUEUE_MAX) // some limit
    {
        DmaQueue::DmaSession blob = { .ptr = const_cast<std::uint8_t*>(ptr), .size = size, .dmaCallbacks = *this };
        txSessionQueue.push_back(blob);
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
        delete[] txSessionQueue.front().ptr;
        txSessionQueue.pop_front();
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

    if(txSessionQueue.empty() == false)
    {
        if (transmitDma(txSessionQueue.front().ptr, 
                        txSessionQueue.front().size, 
                        txSessionQueue.front().dmaCallbacks))
        {
            retval = true;
        }
    }

    return retval;
}

DmaQueue::~DmaQueue()
{
}
