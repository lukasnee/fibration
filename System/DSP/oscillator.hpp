#pragma once

#include "logger.hpp"
#include "map.hpp"
#include "arm_math.h"

#include <array>
namespace Fib
{
    namespace DSP
    {
        class OscillatorF32
        {
        public:
            struct Config
            {
                struct Ranges
                {
                    static constexpr auto amplitudeNormal = Fib::DSP::Range<float>(0.f, 1.f);
                    static constexpr auto frequencyInHz = Fib::DSP::Range<float>(0.f, 20'000.f);
                    static constexpr auto phaseInRad = Fib::DSP::Range<float>(0.f, (2.f * PI));
                };
            };

            OscillatorF32(float sampleRateInHz = 44'100.f, float frequencyInHz = 0.f, float initialPhaseInRad = 0.f)
                : sampleRateInHz(sampleRateInHz), frequencyInHz(frequencyInHz), phaseInRad(initialPhaseInRad){};
            float step()
            {
                const float samplePeriod = 2 * PI / this->sampleRateInHz;
                float phaseShiftInRad;
                arm_scale_f32(&this->frequencyInHz, samplePeriod, &phaseShiftInRad, 1); /* phaseShiftInRad = f * t */
                arm_add_f32(&this->phaseInRad, &phaseShiftInRad, &this->phaseInRad, 1); /* phaseInRad += phaseShiftInRad */
                while (this->phaseInRad > Config::Ranges::phaseInRad.getUpper())
                {
                    this->phaseInRad = this->phaseInRad - Config::Ranges::phaseInRad.getUpper();
                }
                float sinPure = arm_sin_f32(this->phaseInRad);
                float sinAmplified;
                arm_mult_f32(&sinPure, &this->amplitudeNormal, &sinAmplified, 1);
                return sinAmplified;
            }

            template <std::size_t numOfSampleBlocks>
            void step(Fib::DSP::SampleBlocks<float, numOfSampleBlocks> &sampleBlocksOut)
            {
                const float samplePeriod = 2 * PI / this->sampleRateInHz;
                const float phaseDeltaInRad = this->frequencyInHz * samplePeriod;

                Fib::DSP::SampleBlock<float> phaseStepsInRadSampleBlockF32 = {{1.f, 2.f, 3.f, 4.f}};
                arm_scale_f32(phaseStepsInRadSampleBlockF32.data(), phaseDeltaInRad, phaseStepsInRadSampleBlockF32.data(), phaseStepsInRadSampleBlockF32.size());

                for (std::size_t i = 0; i < sampleBlocksOut.size(); i++)
                {
                    /* get sample phases */
                    Fib::DSP::SampleBlock<float> phaseInRadSampleBlockF32;
                    arm_offset_f32(phaseStepsInRadSampleBlockF32.data(), this->phaseInRad, phaseInRadSampleBlockF32.data(), phaseInRadSampleBlockF32.size());

                    /* loop phase around 2*PI and get sine values */
                    Fib::DSP::SampleBlock<float> sampleBlockF32;
                    for (std::size_t j = 0; j < phaseInRadSampleBlockF32.size(); j++)
                    {
                        while (phaseInRadSampleBlockF32[j] > Config::Ranges::phaseInRad.getUpper())
                        {
                            phaseInRadSampleBlockF32[j] = phaseInRadSampleBlockF32[j] - Config::Ranges::phaseInRad.getUpper();
                        }
                        sampleBlockF32[j] = arm_sin_f32(phaseInRadSampleBlockF32[j]);
                    }

                    /* scale to amplitude normal */
                    arm_scale_f32(sampleBlockF32.data(), this->amplitudeNormal, sampleBlocksOut[i].data(), sampleBlocksOut[i].size());
                    this->phaseInRad = phaseInRadSampleBlockF32.back();
                }
            }

            float getAmplitudeNormal() { return this->amplitudeNormal; }
            bool setAmplitudeNormal(float amplitudeNormal)
            {
                bool result = false;
                if (Config::Ranges::amplitudeNormal.isInRange(amplitudeNormal))
                {
                    this->amplitudeNormal = amplitudeNormal;
                    result = true;
                }
                return result;
            }

            float getFrequencyInHz() { return this->frequencyInHz; }
            bool setFrequencyInHz(float frequencyInHz)
            {
                bool result = false;
                if (Config::Ranges::frequencyInHz.isInRange(frequencyInHz))
                {
                    this->frequencyInHz = frequencyInHz;
                    result = true;
                }
                return result;
            }

            float getPhaseInRad() { return this->phaseInRad; }
            bool setPhaseInRad(float phaseInRad)
            {
                bool result = false;
                if (Config::Ranges::phaseInRad.isInRange(phaseInRad))
                {
                    this->phaseInRad = phaseInRad;
                    result = true;
                }
                return result;
            }

        private:
            const float sampleRateInHz = 44'100.f;
            float amplitudeNormal = 0.f;
            float frequencyInHz = 0.f;
            float phaseInRad = 0.f;
            // const OscillatorF32(OscillatorF32 &) = delete;
            OscillatorF32(OscillatorF32 &&) = delete;
        };
    } // namespace DSP
} // namespace Fib

// class OscillatorQ31
// {
// public:
//     OscillatorQ31(float frequencyInHz = 0, float InitialPhaseInRad = 0x80000000) : frequencyInHz(frequencyInHz), phaseInRad(InitialPhaseInRad){};

//     float evaluateStep(float stepInSeconds)
//     {
//         float phaseShift;

//         // 0x1A36E
//         arm_mult_f32(&stepInSeconds, &this->frequencyInHz, &phaseShift, 1); // phaseShift = stepInSeconds * this->frequencyInHz
//         // q31_t divisor = Fib::DSP::floatToQ31(0.00005f);
//         // arm_mult_q31(&phaseShift, &divisor, &phaseShift, 1); // phaseShift = phaseShift * q0.00005

//         this->phaseInRad += phaseShift; // no cmsis add to avoid saturation !
//         // arm_add_q31(&this->phaseInRad, &phaseShift, &this->phaseInRad, 1); // this->phaseInRad += phaseShift
//         float sinPure = arm_sin_f32(this->phaseInRad);
//         float sinAmplified;
//         arm_mult_f32(&sinPure, &this->amplitudeNormal, &sinAmplified, 1); // sinAmplified = sinPure * this->amplitudeNormal

//         //   Logger::log(Logger::Verbosity::low, Logger::Type::trace, "\n"
//         //         "stepInSeconds: %ld\n"
//         //         "phaseShift: %ld\n"
//         //         "this->frequencyInHz:  %ld\n"
//         //         "sinPure:  %ld\n",
//         //         stepInSeconds, phaseShift, this->frequencyInHz, sinPure);

//         return sinAmplified;
//     }

//     float getFrequency() { return frequencyInHz; }
//     void setFrequency(float frequencyInHz) { this->frequencyInHz = frequencyInHz; }
//     void setAmplitude(float amplitude) { this->amplitudeNormal = amplitude; }

// private:
//     float amplitudeNormal = 0;
//     float frequencyInHz = 0;
//     float phaseInRad = 0;

//     //const Oscillator(Oscillator &) = delete;
//     Oscillator(Oscillator &&) = delete;
// };
