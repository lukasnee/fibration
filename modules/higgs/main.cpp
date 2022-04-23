#include "fibration.hpp"

#include <array>
#include <cstdint>
#include <limits>

#include "Drums/analogbassdrum.h"
#include "Drums/analogsnaredrum.h"
#include "Drums/hihat.h"
#include "Drums/synthbassdrum.h"
#include "Drums/synthsnaredrum.h"

static I2sStream::Buffer i2s2StreamBuffer;

I2sStream i2s2Stream(
    Periph::getI2s2(), "i2s2stream", 4 * 1024 / sizeof(StackType_t), FibSys::Priority::audioStream, i2s2StreamBuffer,
    []([[maybe_unused]] const Fib::Dsp::StereoSampleBufferF32 &rxStereoSampleBlock,
       [[maybe_unused]] Fib::Dsp::StereoSampleBufferF32 &txStereoSampleBuffer) {
        static bool hasSetup = false;
        static daisysp::SyntheticBassDrum drum1;
        static daisysp::SyntheticSnareDrum drum2;
        static daisysp::HiHat drum3;
        static daisysp::AnalogBassDrum drum4;
        static daisysp::AnalogSnareDrum drum5;

        auto &drum = drum5;
        if (!hasSetup) {
            drum.Init(44100.f);
            hasSetup = true;
        }

        static auto sineWave = Fib::Dsp::Osc(Fib::Dsp::Osc::Waveforms::sine);
        static auto squareWave = Fib::Dsp::Osc(Fib::Dsp::Osc::Waveforms::square);
        static auto sawWave = Fib::Dsp::Osc(Fib::Dsp::Osc::Waveforms::saw);
        static auto triangeWave = Fib::Dsp::Osc(Fib::Dsp::Osc::Waveforms::triangle);
        static Fib::Dsp::Osc *pWaves[] = {&squareWave, &sawWave, &triangeWave, &sineWave};

        Fib::Dsp::BufferF32<4> potNormal;
        Periph::getAdc2().getNormal(3, potNormal[0]);
        Periph::getAdc2().getNormal(4, potNormal[1]);
        Periph::getAdc2().getNormal(5, potNormal[2]);
        Periph::getAdc2().getNormal(2, potNormal[3]);
        Fib::Dsp::power2(potNormal, potNormal);

        pWaves[0]->frequencyInHz.set(Fib::Dsp::Map::denormalize(potNormal[0], 0.f, 20'000.f));
        pWaves[1]->frequencyInHz.set(Fib::Dsp::Map::denormalize(potNormal[1], 0.f, 20'000.f));
        pWaves[2]->frequencyInHz.set(Fib::Dsp::Map::denormalize(potNormal[2], 0.f, 20'000.f));
        pWaves[3]->frequencyInHz.set(Fib::Dsp::Map::denormalize(potNormal[3], 0.f, 20'000.f));

        // const Fib::Dsp::F32 rolloff = 1.f;
        // pWaves[0]->amplitudeNormal.set(pWaves[0]->frequencyInHz.get() < rolloff ? pWaves[0]->frequencyInHz.get() /
        // rolloff : 1.f); pWaves[1]->amplitudeNormal.set(pWaves[1]->frequencyInHz.get() < rolloff ?
        // pWaves[1]->frequencyInHz.get() / rolloff : 1.f);
        // pWaves[2]->amplitudeNormal.set(pWaves[2]->frequencyInHz.get() < rolloff ? pWaves[2]->frequencyInHz.get() /
        // rolloff : 1.f); pWaves[3]->amplitudeNormal.set(pWaves[3]->frequencyInHz.get() < rolloff ?
        // pWaves[3]->frequencyInHz.get() / rolloff : 1.f);
        // pWaves[0]->amplitudeNormal.set(pWaves[0]->frequencyInHz.get() < rolloff ? 0.f : 1.f);
        // pWaves[1]->amplitudeNormal.set(pWaves[1]->frequencyInHz.get() < rolloff ? 0.f : 1.f);
        // pWaves[2]->amplitudeNormal.set(pWaves[2]->frequencyInHz.get() < rolloff ? 0.f : 1.f);
        // pWaves[3]->amplitudeNormal.set(pWaves[3]->frequencyInHz.get() < rolloff ? 0.f : 1.f);

        // const auto w1 = pWaves[0]->synthesize();
        // const auto w2 = pWaves[1]->synthesize();
        // const auto w3 = pWaves[2]->synthesize();
        // const auto w4 = pWaves[3]->synthesize();

        Fib::Dsp::SampleBufferF32 w5;
        for (std::size_t i = 0; i < w5.size(); i ++) {
            w5[i] = drum.Process();
        }

        static float coeff[] = {0.0000000f,  -0.0004873f, -0.0019827f, -0.0044985f, -0.0079935f, -0.0123743f, -0.0175003f,
                                -0.0231899f, -0.0292296f, -0.0353835f, -0.0414052f, -0.0470488f, -0.0520807f, -0.0562909f,
                                -0.0595026f, -0.0615804f, 0.9366448f,  -0.0620366f, -0.0603974f, -0.0575900f, -0.0537341f,
                                -0.0489929f, -0.0435655f, -0.0376769f, -0.0315679f, -0.0254834f, -0.0196608f, -0.0143188f,
                                -0.0096473f, -0.0057981f, -0.0028780f, -0.0009441f, -0.0000007f};

        // static arm_iir_lattice_instance_f32 iirInstace;
        static arm_fir_instance_f32 firInstace;
        const uint16_t numTaps = sizeof(coeff) / sizeof(Fib::Dsp::F32);
        // const uint32_t blockSize = w1.size();
        // static Fib::Dsp::F32 stateBuffer[numTaps + blockSize - 1];

        // float biquada[] = {0.9603640455554292, -1.9576260295124621, 0.9069191056867634, -1.904255735525569};
        // float biquadb[] = {1, -2, 1, -2};

        static bool filterInitialized = false;
        if (!filterInitialized) {
            filterInitialized = true;
            // arm_iir_lattice_init_f32(&iirInstace, (sizeof(biquada)/sizeof(float)), biquada, biquadb, stateBuffer,
            // blockSize);
            // arm_fir_init_f32(&firInstace, numTaps, coeff, stateBuffer, blockSize);
        }

        Fib::Dsp::SampleBufferF32 mix;
        for (std::size_t i = 0; i < mix.size(); i++) {
            // mix[i] = (w1[i] + w2[i] + w3[i] + w4[i] + w5[i]) / 5.f;
            mix[i] = w5[i];
            // txStereoSampleBuffer.right[j] = (txStereoSampleBuffer.left[j] + w3[j] + w4[j]) / 4.f;
        }
        txStereoSampleBuffer.left = mix;
        // arm_fir_f32(&firInstace, mix.data(), mix.data(), mix.size());
        txStereoSampleBuffer.right = mix;
        // arm_iir_lattice_f32(&iirInstace, mix.data(), mix.data(), mix.size());

        {
            static Fib::PeriodicTimerApp higgsStats("hs", 10.f, [&]() {
                Logger::log(Logger::Verbosity::low, Logger::Type::trace, "\n%8.2f %8.2f %8.2f %8.2f\n%8.2f %8.2f %8.2f %8.2f\n",
                            pWaves[0]->frequencyInHz.get(), pWaves[1]->frequencyInHz.get(), pWaves[2]->frequencyInHz.get(),
                            pWaves[3]->frequencyInHz.get(), pWaves[0]->amplitudeNormal.get(), pWaves[1]->amplitudeNormal.get(),
                            pWaves[2]->amplitudeNormal.get(), pWaves[3]->amplitudeNormal.get());
            });
            static Shell::Command statsCommand(
                "hs", Shell::Command::Helper::Literal::onOffUsage, nullptr, [] ShellCommandFunctionLambdaSignature {
                    return Shell::Command::Helper::onOffCommand([&](bool state) { return higgsStats.setState(state); },
                                                                "higgs stats", ShellCommandFunctionArgs);
                });
            static Shell::Command drumCommand("d", nullptr, nullptr, [&] ShellCommandFunctionLambdaSignature {
                drum.Trig();
                return Shell::Command::Result::ok;
            });
        }
    });
