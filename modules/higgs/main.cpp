#include "fibration.hpp"

#include <array>
#include <cstdint>
#include <limits>

static I2sDuplexStream::CircularStereoBufferU32 i2s2CircularStereoBufferTxU32, i2s2CircularStereoBufferRxU32;

static I2sDuplexStream i2s2DuplexStream(
    Periph::getI2s2(), "i2s2stream", 2 * 1024 / sizeof(StackType_t), FibSys::Priority::audioStream,
    i2s2CircularStereoBufferTxU32, i2s2CircularStereoBufferRxU32, [](DuplexStereoStreamProcessFunctionParams) {
        static auto squareWave = Fib::Dsp::Waveforms::SquareF32();
        static auto sawWave = Fib::Dsp::Waveforms::SawF32();
        static auto triangeWave = Fib::Dsp::Waveforms::TriangeF32();
        static auto sineWave = Fib::Dsp::Waveforms::SineF32();
        static Fib::Dsp::OscF32 *pWaves[] = {&squareWave, &sawWave, &triangeWave, &sineWave};

        Fib::Dsp::QuadF32 potNormal;
        Periph::getAdc2().getNormal(3, potNormal[0]);
        Periph::getAdc2().getNormal(4, potNormal[1]);
        Periph::getAdc2().getNormal(5, potNormal[2]);
        Periph::getAdc2().getNormal(2, potNormal[3]);
        potNormal = Fib::Dsp::Math::secondPower(potNormal);

        pWaves[0]->frequencyInHz.set(Fib::Dsp::Map::denormalize(potNormal[0], 0.f, 20'000.f));
        pWaves[1]->frequencyInHz.set(Fib::Dsp::Map::denormalize(potNormal[1], 0.f, 20'000.f));
        pWaves[2]->frequencyInHz.set(Fib::Dsp::Map::denormalize(potNormal[2], 0.f, 20'000.f));
        pWaves[3]->frequencyInHz.set(Fib::Dsp::Map::denormalize(potNormal[3], 0.f, 20'000.f));

        const Fib::Dsp::F32 rolloff = 1.f;
        // pWaves[0]->amplitudeNormal.set(pWaves[0]->frequencyInHz.get() < rolloff ? pWaves[0]->frequencyInHz.get() /
        // rolloff : 1.f); pWaves[1]->amplitudeNormal.set(pWaves[1]->frequencyInHz.get() < rolloff ?
        // pWaves[1]->frequencyInHz.get() / rolloff : 1.f);
        // pWaves[2]->amplitudeNormal.set(pWaves[2]->frequencyInHz.get() < rolloff ? pWaves[2]->frequencyInHz.get() /
        // rolloff : 1.f); pWaves[3]->amplitudeNormal.set(pWaves[3]->frequencyInHz.get() < rolloff ?
        // pWaves[3]->frequencyInHz.get() / rolloff : 1.f);
        pWaves[0]->amplitudeNormal.set(pWaves[0]->frequencyInHz.get() < rolloff ? 0.f : 1.f);
        pWaves[1]->amplitudeNormal.set(pWaves[1]->frequencyInHz.get() < rolloff ? 0.f : 1.f);
        pWaves[2]->amplitudeNormal.set(pWaves[2]->frequencyInHz.get() < rolloff ? 0.f : 1.f);
        pWaves[3]->amplitudeNormal.set(pWaves[3]->frequencyInHz.get() < rolloff ? 0.f : 1.f);

        for (std::size_t i = 0; i < txLeftSampleBlocksF32.size(); i++)
        {
            const auto w1 = pWaves[0]->generateMult();
            const auto w2 = pWaves[1]->generateMult();
            const auto w3 = pWaves[2]->generateMult();
            const auto w4 = pWaves[3]->generateMult();

            static float coeff[] = {0.0000000f,  -0.0004873f, -0.0019827f, -0.0044985f, -0.0079935f, -0.0123743f,
                                    -0.0175003f, -0.0231899f, -0.0292296f, -0.0353835f, -0.0414052f, -0.0470488f,
                                    -0.0520807f, -0.0562909f, -0.0595026f, -0.0615804f, 0.9366448f,  -0.0620366f,
                                    -0.0603974f, -0.0575900f, -0.0537341f, -0.0489929f, -0.0435655f, -0.0376769f,
                                    -0.0315679f, -0.0254834f, -0.0196608f, -0.0143188f, -0.0096473f, -0.0057981f,
                                    -0.0028780f, -0.0009441f, -0.0000007f};

            // static arm_iir_lattice_instance_f32 iirInstace;
            static arm_fir_instance_f32 firInstace;
            const uint16_t numTaps = sizeof(coeff) / sizeof(Fib::Dsp::F32);
            const uint32_t blockSize = w1.size();
            static Fib::Dsp::F32 stateBuffer[numTaps + blockSize - 1];

            // float biquada[] = {0.9603640455554292, -1.9576260295124621, 0.9069191056867634, -1.904255735525569};
            // float biquadb[] = {1, -2, 1, -2};

            static bool filterInitialized = false;
            if (!filterInitialized)
            {
                filterInitialized = true;
                // arm_iir_lattice_init_f32(&iirInstace, (sizeof(biquada)/sizeof(float)), biquada, biquadb, stateBuffer,
                // blockSize);
                arm_fir_init_f32(&firInstace, numTaps, coeff, stateBuffer, blockSize);
            }
            Fib::Dsp::QuadF32 mix;
            for (std::size_t j = 0; j < mix.size(); j++)
            {
                mix[j] = (w1[j] + w2[j] + w3[j] + w4[j]) / 4.f;
                // txRightSampleBlocksF32[i][j] = (rxLeftSampleBlocksF32[i][j] + w3[j] + w4[j]) / 4.f;
            }
            txLeftSampleBlocksF32[i] = mix;
            arm_fir_f32(&firInstace, mix.data(), mix.data(), mix.size());
            // arm_iir_lattice_f32(&iirInstace, mix.data(), mix.data(), mix.size());
            txRightSampleBlocksF32[i] = mix;
        }

        {
            static Fib::PeriodicTimerApp spiraleStats("ss", 10.f, [&]() {
                Logger::log(
                    Logger::Verbosity::low, Logger::Type::trace, "\n%8.2f %8.2f %8.2f %8.2f\n%8.2f %8.2f %8.2f %8.2f\n",
                    pWaves[0]->frequencyInHz.get(), pWaves[1]->frequencyInHz.get(), pWaves[2]->frequencyInHz.get(),
                    pWaves[3]->frequencyInHz.get(), pWaves[0]->amplitudeNormal.get(), pWaves[1]->amplitudeNormal.get(),
                    pWaves[2]->amplitudeNormal.get(), pWaves[3]->amplitudeNormal.get());
            });
            static Shell::Command statsCommand("ss", Shell::Command::Helper::Literal::onOffUsage, nullptr,
                                               [] ShellCommandFunctionLambdaSignature {
                                                   return Shell::Command::Helper::onOffCommand(
                                                       [&](bool state) -> bool { return spiraleStats.setState(state); },
                                                       "higgs stats", ShellCommandFunctionArgs);
                                               });
        }
    });

class BlinkTestApp : public cpp_freertos::Thread
{
public:
    BlinkTestApp(const char *pcName, uint16_t usStackDepth, UBaseType_t uxPriority)
        : Thread(pcName, usStackDepth, uxPriority)
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
        Gpio::initAsOutput<Gpio::Pin::A5>(Gpio::PinState::low, Gpio::Pull::none);
        DelayUntil(cpp_freertos::Ticks::MsToTicks(1000));
        i2s2DuplexStream.start();
        while (true)
        {
            Gpio::writeLow<Gpio::Pin::A5>();
            DelayUntil(cpp_freertos::Ticks::MsToTicks(50));
            Gpio::writeHigh<Gpio::Pin::A5>();
            DelayUntil(cpp_freertos::Ticks::MsToTicks((50)));
        }
    }
};

BlinkTestApp blinkTestApp("blinkTestApp", 1024 / sizeof(StackType_t), FibSys::Priority::appHigh);
