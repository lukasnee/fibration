#pragma once

#include "timer.hpp"
#include <functional>

namespace Fib
{
class PeriodicTimerApp : public cpp_freertos::Timer
{
public:
    PeriodicTimerApp(const char *strName, float frequencyInHz, std::function<void(void)> callbackF = nullptr)
        : Timer(strName, (TickType_t)(1000.f / frequencyInHz), true), callbackF(callbackF)
    {
    }
    bool setState(bool state)
    {
        return state ? this->Start() : this->Stop();
    }

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

class PeriodicRandomValue : public PeriodicTimerApp
{
public:
    PeriodicRandomValue(float frequencyInHz) : PeriodicTimerApp("PRV", frequencyInHz)
    {
        if (!Start())
        {
            FIBSYS_PANIC();
        }
    }
    float get()
    {
        return this->value;
    }

private:
    virtual void Run() override
    {
        this->value = static_cast<float>(((2.0f * std::rand()) / RAND_MAX) - 1.0f);
        Logger::log(Logger::Verbosity::high, Logger::Type::trace, "periodicRandomValue: %f\n", this->value);
    }
    float value = 0.f;
};

// PeriodicRandomValue periodicRandomValue(1.f);
// static Shell::Command psnCommand("psn", Shell::Command::Helper::Literal::onOffUsage, nullptr,
// [](ShellCommandFunctionParams) {
//     return Shell::Command::Helper::onOffCommand([&](bool state) -> bool { return periodicRandomValue.setState(state);
//     },
//                                                 "psn", ShellCommandFunctionArgs);
// });

} // namespace Fib
