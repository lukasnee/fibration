#include "fibration.hpp"

#include <cstdint>
#include <limits>
#include <array>

class PeriodicRandomValue : public Fib::PeriodicTimerApp
{
public:
    PeriodicRandomValue(float frequencyInHz) : Fib::PeriodicTimerApp("PRV", frequencyInHz)
    {
        if (!Start())
        {
            FIBSYS_PANIC();
        }
    }
    float get() { return this->value; }

private:
    virtual void Run() override
    {
        this->value = static_cast<float>(((2.0f * std::rand()) / RAND_MAX) - 1.0f);
        Logger::log(Logger::Verbosity::high, Logger::Type::trace, "periodicRandomValue: %f\n", this->value);
    }
    float value = 0.f;
} periodicRandomValue(1.f);

static Shell::Command psnCommand(
    "psn", Shell::Command::Helper::Literal::onOffUsage, nullptr, [](SHELLCMDPARAMS)
    { return Shell::Command::Helper::onOffCommand([&](bool state) -> bool
                                                  { return periodicRandomValue.setState(state); },
                                                  "psn", SHELLCMDARGS); });

static I2sDuplexStream::CircularStereoBufferU32
    i2s2CircularStereoBufferTxU32,
    i2s2CircularStereoBufferRxU32;

static I2sDuplexStream i2s2DuplexStream(
    Periph::getI2s2(), "i2s2stream", 1024 / sizeof(StackType_t), FibSys::Priority::audioStream,
    i2s2CircularStereoBufferTxU32, i2s2CircularStereoBufferRxU32,
    [](const I2sDuplexStream::SampleBlocksF32 &rxLeftSampleBlocksF32, const I2sDuplexStream::SampleBlocksF32 &rxRightSampleBlocksF32,
       I2sDuplexStream::SampleBlocksF32 &txLeftSampleBlocksF32, I2sDuplexStream::SampleBlocksF32 &txRightSampleBlocksF32)
    {
        static auto sineF32A = Fib::DSP::Osc::SineF32();
        static auto sineF32B = Fib::DSP::Osc::SineF32();

        std::array<float, 4> potValues;
        Periph::getAdc2().getValue(5, potValues[0]);
        Periph::getAdc2().getValue(2, potValues[1]);
        Periph::getAdc2().getValue(3, potValues[2]);
        Periph::getAdc2().getValue(4, potValues[3]);

        arm_mult_f32(potValues.data(), potValues.data(), potValues.data(), potValues.size());
        arm_mult_f32(potValues.data(), potValues.data(), potValues.data(), potValues.size());

        sineF32A.frequencyInHz.set(Fib::DSP::map(potValues[0], 0.f, 1.f, 0.f, 20'000.f));
        sineF32A.amplitudeNormal.set(potValues[1]);
        sineF32A.generateMult(txLeftSampleBlocksF32.data(), txLeftSampleBlocksF32.size());

        sineF32B.frequencyInHz.set(Fib::DSP::map(potValues[2], 0.f, 1.f, 0.f, 20'000.f));
        sineF32B.amplitudeNormal.set(potValues[3]);
        sineF32B.generateMult(txRightSampleBlocksF32.data(), txRightSampleBlocksF32.size());

        // txRightSampleBlocksF32 = rxLeftSampleBlocksF32;

        {
            static Fib::PeriodicTimerApp spiraleStats(
                "ss", 10.f, [&]()
                { Logger::log(Logger::Verbosity::low, Logger::Type::trace, "%f,%f,%f,%f\n",
                              sineF32A.frequencyInHz.get(),
                              sineF32A.amplitudeNormal.get(),
                              sineF32B.frequencyInHz.get(),
                              sineF32B.amplitudeNormal.get()); });
            static Shell::Command statsCommand(
                "ss", Shell::Command::Helper::Literal::onOffUsage, nullptr, [](SHELLCMDPARAMS)
                { return Shell::Command::Helper::onOffCommand([&](bool state) -> bool
                                                              { return spiraleStats.setState(state); },
                                                              "higgs stats", SHELLCMDARGS); });
        }
    });

class BlinkTestApp : public cpp_freertos::Thread
{
public:
    BlinkTestApp(const char *pcName, uint16_t usStackDepth, UBaseType_t uxPriority) : Thread(pcName, usStackDepth, uxPriority)
    {
        if (!Start())
        {
            FIBSYS_PANIC();
        }
    };

private:
    virtual void Run() override
    {
        Logger::log(Logger::Verbosity::high, Logger::Type::trace, "BlinkTestApp started\n");
        Gpio &onBoardLed = Periph::getGpio(Gpio::Port::A, Gpio::Pin::pin5);
        onBoardLed.init(Gpio::Mode::outputPushPull, Gpio::PinState::low);
        DelayUntil(cpp_freertos::Ticks::MsToTicks(1000));
        i2s2DuplexStream.start();
        while (true)
        {
            onBoardLed.write(Gpio::PinState::low);
            DelayUntil(cpp_freertos::Ticks::MsToTicks(50));
            onBoardLed.write(Gpio::PinState::high);
            DelayUntil(cpp_freertos::Ticks::MsToTicks((50)));
        }
    }
};

BlinkTestApp blinkTestApp("blinkTestApp", 1024 / sizeof(StackType_t), FibSys::Priority::appHigh);
