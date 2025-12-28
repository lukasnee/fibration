#pragma once

#include "FreeRTOS/Timer.hpp"
#include "ln/logger/logger.hpp"

#include <functional>

namespace Fib {
class PeriodicTimerApp : public FreeRTOS::Timer {
public:
    PeriodicTimerApp(const char *strName, float frequencyInHz, std::function<void(void)> callbackF = nullptr)
        : Timer((TickType_t)(1000.f / frequencyInHz), true, strName), callbackF(callbackF) {}
    bool setState(bool state) { return state ? this->start() : this->stop(); }

private:
    virtual void timerFunction() override {
        if (this->callbackF) {
            this->callbackF();
        }
    }
    std::function<void(void)> callbackF;
};

class PeriodicRandomValue : public PeriodicTimerApp {
public:
    PeriodicRandomValue(float frequencyInHz) : PeriodicTimerApp("PRV", frequencyInHz) {
        if (!this->start()) {
            LN_PANIC();
        }
    }
    float get() { return this->value; }

private:
    LOG_MODULE_CLASS_MEMBER("periodicTimerApp", LOGGER_LEVEL_NOTSET);

    virtual void timerFunction() override {
        this->value = static_cast<float>(((2.0f * std::rand()) / RAND_MAX) - 1.0f);
        LOG_INFO("periodicRandomValue: %f\n", this->value);
    }
    float value = 0.f;
};

} // namespace Fib
