#pragma once

#include "thread.hpp"
#include "status.hpp"

#include <cstdint>

class FibSys : public cpp_freertos::Thread
{
public:
    // entry point of fibration system, should be called in main
    static void boot();

    // 1KHz system tick time reference
    static std::uint32_t getSysTick();

    // should be called in case fatal error, could be used as run-time assert
    static void panic();

    static constexpr BaseType_t getSysMaxPriority() { return configMAX_PRIORITIES - 1; } 
    static constexpr BaseType_t getSysAvgPriority() { return (getSysMaxPriority() / 2); }
    static constexpr BaseType_t getAudioPriority() { return (getSysAvgPriority() + 0); } // TODO: figure out what's better
    static constexpr BaseType_t getAppMaxPriority() { return (getSysAvgPriority() - 0); } // TODO: figure out what's better
    //void collectStats();
    Stats stats;

    FibSys(const FibSys &) = delete;
    FibSys(FibSys &&) = delete;

private:
    // can only be instantiated with FibSys::boot()
    FibSys(std::uint16_t stackDepth, BaseType_t priority);

    // FibSys thread code
    virtual void Run() override;
};
