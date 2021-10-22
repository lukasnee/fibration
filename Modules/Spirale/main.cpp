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

static I2sDuplexStream::CircularBuffer i2s2DuplexStreamCircularBufferTx, i2s2DuplexStreamCircularBufferRx;

static I2sDuplexStream i2s2DuplexStream(
    Periph::getI2s2(), "i2s2stream", 1024 / sizeof(StackType_t), FibSys::Priority::audioStream,
    i2s2DuplexStreamCircularBufferTx, i2s2DuplexStreamCircularBufferRx,
    [](const DuplexStereoStreamIF::Buffer &rxBuffer, DuplexStereoStreamIF::Buffer &txBuffer)
    {

        static auto osc1 = Fib::DSP::OscillatorF32();
        static auto osc2 = Fib::DSP::OscillatorF32();

        std::array<float, 4> potValues;
        Periph::getAdc2().getValue(5, potValues[0]);
        Periph::getAdc2().getValue(2, potValues[1]);
        Periph::getAdc2().getValue(3, potValues[2]);
        Periph::getAdc2().getValue(4, potValues[3]);

        arm_mult_f32(potValues.data(), potValues.data(), potValues.data(), potValues.size());
        arm_mult_f32(potValues.data(), potValues.data(), potValues.data(), potValues.size());

        osc1.setFrequencyInHz(Fib::DSP::map(potValues[0], 0.f, 1.f, 0.f, 20'000.f));
        osc1.setAmplitudeNormal(potValues[1]);
        osc2.setFrequencyInHz(Fib::DSP::map(potValues[2], 0.f, 1.f, 0.f, 20'000.f));
        osc2.setAmplitudeNormal(potValues[3]);

        Fib::DSP::SampleBlocks<float, 8> ch1SampleBlocksF32;
        osc1.step(ch1SampleBlocksF32);

        Fib::DSP::SampleBlocks<float, 8> ch2SampleBlocksF32;
        osc2.step(ch2SampleBlocksF32);

        for (std::size_t i = 0; i < ch2SampleBlocksF32.size(); i++)
        {
            for (std::size_t j = 0; j < ch2SampleBlocksF32.front().size(); j++)
            {
                auto ch1SampleQ31 = Fib::DSP::floatToQ31(ch1SampleBlocksF32[i][j]) >> 8;
                auto ch2SampleQ31 = Fib::DSP::floatToQ31(ch2SampleBlocksF32[i][j]) >> 8;
                txBuffer[i * ch1SampleBlocksF32.front().size() + j].left = Fib::DSP::swap(ch1SampleQ31);
                txBuffer[i * ch2SampleBlocksF32.front().size() + j].right = Fib::DSP::swap(ch2SampleQ31);
            }
        }

        // dry pass ADC LEFT -> DAC RIGHT
        // for (std::size_t i = 0; i < txBuffer.size(); i++)
        // {
        //     txBuffer[i].right = rxBuffer[i].left;
        // }

        {
            static Fib::PeriodicTimerApp spiraleStats(
                "ss", 10.f, [&]()
                { Logger::log(Logger::Verbosity::low, Logger::Type::trace, "%f,%f,%f,%f\n",
                              osc1.getFrequencyInHz(),
                              osc1.getAmplitudeNormal(),
                              osc2.getFrequencyInHz(),
                              osc2.getAmplitudeNormal()); });
            static Shell::Command statsCommand(
                "ss", Shell::Command::Helper::Literal::onOffUsage, nullptr, [](SHELLCMDPARAMS)
                { return Shell::Command::Helper::onOffCommand([&](bool state) -> bool
                                                              { return spiraleStats.setState(state); },
                                                              "spirale stats", SHELLCMDARGS); });
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
