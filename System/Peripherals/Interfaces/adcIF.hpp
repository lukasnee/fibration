 
/*
    defines the required interface for ADC implementation 
*/
#pragma once
#include <cstdint>
#include <semaphore.hpp>

class AdcIF
{
public:
    bool getValue(std::uint32_t channelNo, float &value)
    {
        this->txBinarySemaphore.Take();
        bool retval = false;

        if (getValueUnsafe(channelNo, value))
        {
            retval = true;
        }

        this->txBinarySemaphore.Give();
        return retval;
    }

    std::uint32_t getMaxValue() { return ((1 << this->getBitDepth()) - 1); }

    AdcIF() : txBinarySemaphore(true){};
    ~AdcIF(){};

    virtual std::uint32_t getBitDepth() = 0;
    virtual std::uint32_t getChannelsTotal() = 0;
    virtual bool init() = 0;
    virtual bool deinit() = 0;

protected:
    virtual bool getValueUnsafe(std::uint32_t channelNo, float &value) = 0;

private:
    cpp_freertos::BinarySemaphore txBinarySemaphore;
};