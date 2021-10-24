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
          frequencyInHz(frequencyInHz),
          phaseInRad(initialPhaseInRad){};

    void OscF32::generateMult(SampleBlock<F32> *pSampleBlocksOut, std::size_t sampleBlocksSize)
    {
        for (std::size_t i = 0; i < sampleBlocksSize; i++)
        {
            this->generate(pSampleBlocksOut[i]);
        }
    }

    F32 OscF32::getAmplitudeNormal() { return this->amplitudeNormal; }
    bool OscF32::setAmplitudeNormal(F32 amplitudeNormal)
    {
        bool result = false;
        if (Config::Ranges::amplitudeNormal.isInRange(amplitudeNormal))
        {
            this->amplitudeNormal = amplitudeNormal;
            result = true;
        }
        return result;
    }

    F32 OscF32::getFrequencyInHz() { return this->frequencyInHz; }
    bool OscF32::setFrequencyInHz(F32 frequencyInHz)
    {
        bool result = false;
        if (Config::Ranges::frequencyInHz.isInRange(frequencyInHz))
        {
            this->frequencyInHz = frequencyInHz;
            this->config.derived.derive(frequencyInHz);
            result = true;
        }
        return result;
    }

    F32 OscF32::getPhaseInRad() { return this->phaseInRad; }
    bool OscF32::setPhaseInRad(F32 phaseInRad)
    {
        bool result = false;
        if (Config::Ranges::phaseInRad.isInRange(phaseInRad))
        {
            this->phaseInRad = phaseInRad;
            result = true;
        }
        return result;
    }

    SineF32::SineF32(F32 sampleRateInHz, F32 frequencyInHz, F32 initialPhaseInRad)
        : OscF32(sampleRateInHz, frequencyInHz, initialPhaseInRad){};

    void SineF32::generate(SampleBlock<F32> &sampleBlocksOut)
    {
        /* get sample phases */
        Fib::DSP::SampleBlock<F32> phaseInRadSampleBlockF32;
        arm_offset_f32(this->config.derived.getSampleBlockDeltaTimeScale().data(), this->getPhaseInRad(),
                       phaseInRadSampleBlockF32.data(), phaseInRadSampleBlockF32.size());

        /* loop phase around 2*PI and get sine values */
        Fib::DSP::SampleBlock<F32> sampleBlockF32;
        for (std::size_t j = 0; j < phaseInRadSampleBlockF32.size(); j++)
        {
            while (phaseInRadSampleBlockF32[j] > Config::Ranges::phaseInRad.getUpper())
            {
                phaseInRadSampleBlockF32[j] = phaseInRadSampleBlockF32[j] - Config::Ranges::phaseInRad.getUpper();
            }
            sampleBlockF32[j] = arm_sin_f32(phaseInRadSampleBlockF32[j]);
        }

        /* scale to amplitude normal */
        arm_scale_f32(sampleBlockF32.data(), this->getAmplitudeNormal(), sampleBlocksOut.data(), sampleBlocksOut.size());
        this->setPhaseInRad(phaseInRadSampleBlockF32.back());
    }

} // Fib::DSP::Oscillator
