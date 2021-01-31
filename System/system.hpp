#pragma once

#include "thread.hpp"

#include <cstdint>
class FibSys : public cpp_freertos::Thread
{
public:
    // entry point of fibration system, should be called in main
    static void start();

    // 1KHz system tick time reference
    static std::uint32_t getSysTick();

    // should be called in case fatal error, could be used as run-time assert
    static void panic();
    static BaseType_t getSysMaxPriority();
    static BaseType_t getAppMaxPriority();
    //void collectStats();
    
    
    FibSys(const FibSys &) = delete;
    FibSys(FibSys &&) = delete;
private:
    // can only be instantiated with FibSys::start()
    FibSys(std::uint16_t stackDepth, BaseType_t priority);

    // FibSys thread code
    virtual void Run() override;
};
