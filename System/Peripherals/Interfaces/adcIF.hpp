
/*
    defines the required interface for ADC implementation 
*/
#pragma once
#include <semaphore.hpp>
// namespace CMSIS
// {
extern "C"
{
#include "arm_math.h"
}
// } // namespace CMSIS

#include <cstdint>
#include <limits>

class AdcIF
{
public:
    virtual bool init() = 0;
    virtual bool start() = 0;
    virtual std::uint32_t getChannelsTotal() = 0;
    virtual std::uint32_t getBitDepth() = 0;
    virtual std::uint32_t getFrameBitWidth() = 0;

    std::uint32_t getMaxValue() { return ((1 << this->getBitDepth()) - 1); }

    bool getValue(std::size_t channelNo, q31_t &valueOut)
    {
        bool retval = false;
        std::uint32_t valueRaw;
        if (this->getValueRaw(channelNo, valueRaw))
        {
            valueOut = reinterpret_cast<q31_t>(static_cast<std::int32_t>(valueRaw - (this->getMaxValue() / 2)) * static_cast<std::int32_t>(0xFFFF'FFFF / this->getMaxValue()));
            retval = true;
        }
        return retval;
    }

    bool getValue(std::size_t channelNo, float &valueOut)
    {
        bool retval = false;
        std::uint32_t valueRaw;
        if (this->getValueRaw(channelNo, valueRaw))
        {
            valueOut = static_cast<float>(valueRaw) / static_cast<float>(this->getMaxValue());
            retval = true;
        }
        return retval;
    }

    virtual bool stop() = 0;
    virtual bool deinit() = 0;

protected:
    AdcIF() : resourceSemaphore(true){};
    virtual ~AdcIF(){};

    bool getValueRaw(std::size_t channelNo, std::uint32_t &valueRawOut)
    {
        bool result = false;
        this->resourceSemaphore.Take();
        if (this->getValueUnsafe(channelNo, valueRawOut))
        {
            result = true;
        }
        this->resourceSemaphore.Give();
        return result;
    }

    virtual bool getValueUnsafe(std::size_t channelNo, std::uint32_t &valueOut) = 0;

private:
    cpp_freertos::BinarySemaphore resourceSemaphore;
};