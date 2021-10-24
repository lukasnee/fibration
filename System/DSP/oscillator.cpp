#include "oscillator.hpp"

#include "arm_math.h"

namespace Fib::DSP::Osc
{
    OscF32::Config::Config(F32 sampleRateInHz, F32 frequencyInHz)
        : sampleRateInHz(sampleRateInHz), derived(sampleRateInHz, frequencyInHz)
    {
    }

    OscF32::Config::Derived::Derived(F32 sampleRateInHz, F32 frequencyInHz)
    {
        this->samplePeriodInSec = 2 * PI / sampleRateInHz;
        this->derive(frequencyInHz);
    }

    void OscF32::Config::Derived::derive(F32 frequencyInHz)
    {
        this->phaseDeltaInRad = frequencyInHz * this->samplePeriodInSec;
        this->sampleBlockDeltaTimeScale = {{1.f, 2.f, 3.f, 4.f}};
        arm_scale_f32(this->sampleBlockDeltaTimeScale.data(), phaseDeltaInRad,
                      this->sampleBlockDeltaTimeScale.data(), this->sampleBlockDeltaTimeScale.size());
    }

    OscF32::OscF32(F32 sampleRateInHz, F32 frequencyInHz, F32 initialPhaseInRad)
        : config(sampleRateInHz, frequencyInHz),
          amplitudeNormal(0.f, 0.f, 1.f),
          frequencyInHz(0.f, 0.f, 20'000.f, [&](F32 const &frequencyInHz)
                        {
                            this->config.derived.derive(frequencyInHz);
                            return true;
                        }),
          phaseInRad(0.f, 0.f, (2.f * PI)){};

    void OscF32::generateMult(SampleBlock<F32> *pSampleBlocksOut, std::size_t sampleBlocksSize)
    {
        for (std::size_t i = 0; i < sampleBlocksSize; i++)
        {
            this->generate(pSampleBlocksOut[i]);
        }
    }

    SineF32::SineF32(F32 sampleRateInHz, F32 frequencyInHz, F32 initialPhaseInRad)
        : OscF32(sampleRateInHz, frequencyInHz, initialPhaseInRad){};

    void SineF32::generate(SampleBlock<F32> &sampleBlocksOut)
    {
        /* get sample phases */
        Fib::DSP::SampleBlock<F32> phaseInRadSampleBlockF32;
        arm_offset_f32(this->config.derived.getSampleBlockDeltaTimeScale().data(), this->phaseInRad.get(),
                       phaseInRadSampleBlockF32.data(), phaseInRadSampleBlockF32.size());

        /* loop phase around 2*PI and get sine values */
        Fib::DSP::SampleBlock<F32> sampleBlockF32;
        for (std::size_t j = 0; j < phaseInRadSampleBlockF32.size(); j++)
        {
            while (phaseInRadSampleBlockF32[j] > this->phaseInRad.getUpperLimit())
            {
                phaseInRadSampleBlockF32[j] = phaseInRadSampleBlockF32[j] - this->phaseInRad.getUpperLimit();
            }
            sampleBlockF32[j] = arm_sin_f32(phaseInRadSampleBlockF32[j]);
        }

        /* scale to amplitude normal */
        arm_scale_f32(sampleBlockF32.data(), this->amplitudeNormal.get(), sampleBlocksOut.data(), sampleBlocksOut.size());
        this->phaseInRad.set(phaseInRadSampleBlockF32.back());
    }

} // Fib::DSP::Oscillator
