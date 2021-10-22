#pragma once

#include <functional>
#include "timer.hpp"

namespace Fib
{
    class PeriodicTimerApp : public cpp_freertos::Timer
    {
    public:
        PeriodicTimerApp(const char *strName, float frequencyInHz, std::function<void(void)> callbackF = nullptr)
            : Timer(strName, (TickType_t)(1000.f / frequencyInHz), true), callbackF(callbackF)
        {
        }
        bool setState(bool state) { return state ? this->Start() : this->Stop(); }

    private:
        virtual void Run() override
        {
            if (this->callbackF)
            {
                this->callbackF();
            }
        }
        std::function<void(void)> callbackF;
    };
} // namespace Fib
