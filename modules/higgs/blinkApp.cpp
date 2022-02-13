#include "fibration.hpp"

extern I2sDuplexStream i2s2DuplexStream;

class BlinkTestApp : public cpp_freertos::Thread {
public:
    BlinkTestApp(const char *pcName, uint16_t usStackDepth, UBaseType_t uxPriority) : Thread(pcName, usStackDepth, uxPriority) {
        if (!Start()) {
            FIBSYS_PANIC();
        }
    };

private:
    virtual void Run() override {
        Logger::log(Logger::Verbosity::high, Logger::Type::trace, "BlinkTestApp started\n");
        Gpio::initAsOutput<Gpio::Pin::A5>(Gpio::PinState::low, Gpio::Pull::none);
        DelayUntil(cpp_freertos::Ticks::MsToTicks(1000));
        i2s2DuplexStream.start();
        while (true) {
            Gpio::writeLow<Gpio::Pin::A5>();
            DelayUntil(cpp_freertos::Ticks::MsToTicks(50));
            Gpio::writeHigh<Gpio::Pin::A5>();
            DelayUntil(cpp_freertos::Ticks::MsToTicks((50)));
        }
    }
};

BlinkTestApp blinkTestApp("blinkTestApp", 1024 / sizeof(StackType_t), FibSys::Priority::appHigh);
