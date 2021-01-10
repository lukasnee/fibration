#include "dmaQueue.hpp"

#define QUEUE_MAX 0x40

DmaQueue::DmaQueue(uint32_t priority) : 
    Thread{"dmaQueue", 0x100, priority}, //todo priority move out
    txCpltCount(0)
{
    Thread::Start();
}

void DmaQueue::signalTxHandling(bool callingFromISR)
{
    // TODO: investigate more, problem is that freertos fails if calling alloc/free from ISR 
    if(callingFromISR)
    {
        Thread::ResumeFromISR();
        Thread::Yield(); // TODO: should not it be YieldFromISR ?
    }
    else
    {
        txHandle(); // handle immediately 
    }
};

bool DmaQueue::txPush(const std::uint8_t * ptr, std::uint16_t size, bool callingFromISR, bool isDataStatic)
{
    bool retval = false;

    if(txQueue.size() < QUEUE_MAX) // some limit
    {
        DmaQueue::DmaSession blob = { .ptr = const_cast<std::uint8_t*>(ptr), .size = size, .isDataStatic = isDataStatic, .dmaCallbacks = *this };
        txQueue.push(blob);
        signalTxHandling(callingFromISR);
        retval = true;
    }

    return retval;
}

void DmaQueue::dmaTxCpltCallback()
{
    txCpltCount++;
    signalTxHandling(true);
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
        if(txQueue.front().isDataStatic == false)
        {
            delete[] txQueue.front().ptr;
        }
        
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
