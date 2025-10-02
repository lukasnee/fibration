#include "fibration.hpp"

extern I2sStream i2s2Stream;

LOG_MODULE("BlinkTestApp", LOGGER_LEVEL_NOTSET);

class BlinkTestApp : public FreeRTOS::Task {
public:
    BlinkTestApp(const char *pcName, uint16_t usStackDepth, UBaseType_t uxPriority)
        : Task(uxPriority, usStackDepth, pcName){};

private:
    virtual void taskFunction() override {
        LOG_INFO("BlinkTestApp started");
        Gpio::initAsOutput<Gpio::Pin::A5>(Gpio::PinState::low, Gpio::Pull::none);
        using namespace std::chrono_literals;
        this->delayUntil(1000ms);
        i2s2Stream.start();
        while (true) {
            Gpio::writeLow<Gpio::Pin::A5>();
            this->delayUntil(50ms);
            Gpio::writeHigh<Gpio::Pin::A5>();
            this->delayUntil(50ms);
        }
    }
};

BlinkTestApp blinkTestApp("blinkTestApp", 1024 / sizeof(StackType_t), FibSys::Priority::appHigh);
