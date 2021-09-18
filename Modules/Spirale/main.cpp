#include "fibration.hpp"
#include "oscillator.hpp"
#include "map.hpp"

#include <cstdint>

#include <limits>
#include <array>

#include "../System/Streams/i2sDuplexStream.hpp"

// float randomF = 0.0f;

// class Timah : public cpp_freertos::Timer
// {
// public:
//     Timah() : Timer("snh", 1000, true) {}

// private:
//     void Run() override
//     {
//         randomF = static_cast<float>(((2.0f * std::rand()) / RAND_MAX) - 1.0f);
//     }
// };

// Timah timah;

static I2sDuplexStream::CircularBuffer i2s2DuplexStreamCircularBufferTx;
static I2sDuplexStream::CircularBuffer i2s2DuplexStreamCircularBufferRx;

static void processTxRxBufferI2s2(const DuplexStereoStreamIF::Buffer *pStereoAudioRxBuffer, DuplexStereoStreamIF::Buffer *pStereoAudioTxBuffer)
{
    constexpr float samplingRate = 44100.f;  // [1/sec]
    constexpr float dt = 1.f / samplingRate; //1.0f / samplingRate; // [sec]

    static auto main = OscillatorF32();
    static auto mod = OscillatorF32();

    if (pStereoAudioTxBuffer != nullptr)
    {
       static std::array<float, 4> potValues;
        Periph::getAdc2().getValue(5, potValues[0]);
        Periph::getAdc2().getValue(2, potValues[1]);
        Periph::getAdc2().getValue(3, potValues[2]);
        Periph::getAdc2().getValue(4, potValues[3]);

        arm_mult_f32(potValues.data(), potValues.data(), potValues.data(), potValues.size());
        arm_mult_f32(potValues.data(), potValues.data(), potValues.data(), potValues.size());

        main.setFrequencyInHz(Fib::DSP::map(potValues[0], 1.f, 20'000.f));
        main.setAmplitudeNormal(potValues[1]);
        mod.setFrequencyInHz(Fib::DSP::map(potValues[2], 1.f, 20'000.f));
        mod.setAmplitudeNormal(potValues[3]);

        static std::size_t logPrescaler = 0;
        if (logPrescaler % 500 == 0)
        {
            Logger::log(Logger::Verbosity::low, Logger::Type::trace, "%f,%f,%f,%f\n",
                        main.getFrequencyInHz(),
                        main.getAmplitudeNormal(),
                        mod.getFrequencyInHz(),
                        mod.getAmplitudeNormal());
        }
        // 1KHz FM modulation to DAC LEFT
        for (auto &stereoSample : *pStereoAudioTxBuffer)
        {

            auto outf32 = (main.step(dt) + mod.step(dt)) / 2.f;
            auto outQ31 = Fib::DSP::floatToQ31(outf32) >> 8;

            // auto outSample = static_cast<std::uint32_t>(Fib::DSP::map(outf32, -1.f, 1.f, 0.f, static_cast<float>(Fib::DSP::bitDepthToMaxValue<24>())));
            // auto outSample = Fib::DSP::q31ToSample<24>(outQ31);
            stereoSample.left = Fib::DSP::swap(outQ31);
            static float time = 0.f;
            time += dt;
            logPrescaler++;
            if (logPrescaler % 5000 == 0)
            {
                // Logger::log(Logger::Verbosity::low, Logger::Type::trace, "%+f,%+ld,%lu,%lu,%f\n", outf32, outQ31, outSample, stereoSample.left, time);
            }
            // stereoSample.right = 0;
        }

        // Logger::log("fmSample: %lu\n",  *pStereoAudioTxBuffer);

        // dry pass ADC LEFT -> DAC RIGHT
        for (std::size_t i = 0; i < pStereoAudioTxBuffer->size(); i++)
        {
            (*pStereoAudioTxBuffer)[i].right = (*pStereoAudioRxBuffer)[i].left;
        }
    }
}
/*__attribute__((section(".ccmram")))*/
static I2sDuplexStream i2s2DuplexStream(Periph::getI2s2(),
                                        "i2s2stream",
                                        0x500,
                                        FibSys::Priority::audioStream,
                                        i2s2DuplexStreamCircularBufferTx,
                                        i2s2DuplexStreamCircularBufferRx,
                                        processTxRxBufferI2s2);

class BlinkTestApp : public cpp_freertos::Thread
{
public:
    BlinkTestApp(const char *pcName, uint16_t usStackDepth, UBaseType_t uxPriority) : Thread(pcName, usStackDepth, uxPriority)
    {
        if (!Start())
        {
            FibSys::panic();
        }
    };

private:
    char buffer[0x200];

    virtual void Run() override
    {
        Logger::log(Logger::Verbosity::high, Logger::Type::trace, "BlinkTestApp started\n");
        Gpio &onBoardLed = Periph::getGpio(Gpio::Port::A, Gpio::Pin::pin5);
        onBoardLed.init(Gpio::Mode::outputPushPull, Gpio::PinState::low);

        //Gpio &rotaryButton = Periph::getGpio(Gpio::Port::C, Gpio::Pin::pin14);
        //rotaryButton.init(Gpio::Mode::input, Gpio::Pull::up);

        DelayUntil(cpp_freertos::Ticks::MsToTicks(1000));

        i2s2DuplexStream.start();
        //timah.Start();
        while (true)
        {
            // rotaryButton.read() ? Logger::setColoring() : Logger::colorDisable(); // todo config over uart

            onBoardLed.write(Gpio::PinState::low);
            DelayUntil(cpp_freertos::Ticks::MsToTicks(50));
            onBoardLed.write(Gpio::PinState::high);
            DelayUntil(cpp_freertos::Ticks::MsToTicks((50)));
        }
    }
};

BlinkTestApp blinkTestApp("App1", 0x500, FibSys::Priority::appHigh);

// for (int y = 0; y < i; y++)
// {
//     Logger::trace("mainTask", "%.*s", y, "=================================================================================");
// }
// i++;