#pragma once

extern "C"
{
#include "system.h"
}

#include "status.hpp"
#include "thread.hpp"

#include <cstdint>

class FibSys : public cpp_freertos::Thread {
public:
    // entry point of fibration system, should be called in main
    static void boot();

    static void hardwareReboot();

    // 1KHz system tick time reference
    static std::uint32_t getSysTick();
    static std::uint32_t getUptimeInMs();
    static void getUptime(std::uint32_t &days, std::uint32_t &hours, std::uint32_t &minutes, std::uint32_t &seconds,
                          std::uint32_t &milliseconds);

    enum Priority : BaseType_t {
        lowest = 0,
        veryLow = 1,
        low = 2,
        medium = 3,
        high = 4,
        veryHigh = 5,
        highest = 6,

        // configuration
        appLow = lowest,
        appMedium = veryLow,
        appHigh = low,
        sysLow = medium,
        sysMedium = high,
        sysHigh = veryHigh,
        audioStream = sysLow,

        _enumSize
    };

    static_assert(Priority::_enumSize <= configMAX_PRIORITIES, "invalid priority configration!");

    // void collectStats();
    Stats stats;

    FibSys(const FibSys &) = delete;
    FibSys(FibSys &&) = delete;

private:
    // can only be instantiated with FibSys::boot()
    FibSys(std::uint16_t stackDepth, BaseType_t priority);

    void startup();

    // FibSys thread code
    virtual void Run() override;
};
