#include "waveforms.hpp"

namespace Fib::Dsp::Waveforms
{

SineF32::SineF32(F32 sampleRateInHz, F32 frequencyInHz, F32 initialPhaseInRad)
    : OscF32(sampleRateInHz, frequencyInHz, initialPhaseInRad){};

void SineF32::generate(SampleBlock<F32> &sampleBlockOut)
{
    /* get sample phases */
    Fib::Dsp::SampleBlock<F32> phaseInRadSampleBlockF32;
    arm_offset_f32(this->config.derived.getSampleBlockDeltaTimeScale().data(), this->phaseInRad.get(),
                   phaseInRadSampleBlockF32.data(), phaseInRadSampleBlockF32.size());

    /* loop phase around 2*PI and get sine values */
    Fib::Dsp::SampleBlock<F32> sampleBlockF32;
    for (std::size_t j = 0; j < phaseInRadSampleBlockF32.size(); j++)
    {
        while (phaseInRadSampleBlockF32[j] > this->phaseInRad.getUpperLimit())
        {
            phaseInRadSampleBlockF32[j] = phaseInRadSampleBlockF32[j] - this->phaseInRad.getUpperLimit();
        }
        sampleBlockF32[j] = arm_sin_f32(phaseInRadSampleBlockF32[j]);
    }

    if (this->amplitudeNormal.get() != 1.f)
    {
        /* scale to amplitude normal */
        arm_scale_f32(sampleBlockF32.data(), this->amplitudeNormal.get(), sampleBlockF32.data(), sampleBlockOut.size());
    }
    sampleBlockOut = sampleBlockF32;
    this->phaseInRad.set(phaseInRadSampleBlockF32.back());
}

SquareF32::SquareF32(F32 sampleRateInHz, F32 frequencyInHz, F32 initialPhaseInRad)
    : OscF32(sampleRateInHz, frequencyInHz, initialPhaseInRad){};

void SquareF32::generate(SampleBlock<F32> &sampleBlockOut)
{
    /* get sample phases */
    Fib::Dsp::SampleBlock<F32> phaseInRadSampleBlockF32;
    arm_offset_f32(this->config.derived.getSampleBlockDeltaTimeScale().data(), this->phaseInRad.get(),
                   phaseInRadSampleBlockF32.data(), phaseInRadSampleBlockF32.size());

    /* loop phase around 2*PI and get sine values */
    Fib::Dsp::SampleBlock<F32> sampleBlockF32;
    for (std::size_t j = 0; j < phaseInRadSampleBlockF32.size(); j++)
    {
        while (phaseInRadSampleBlockF32[j] > this->phaseInRad.getUpperLimit())
        {
            phaseInRadSampleBlockF32[j] = phaseInRadSampleBlockF32[j] - this->phaseInRad.getUpperLimit();
        }
        sampleBlockF32[j] = phaseInRadSampleBlockF32[j] < PI ? 1.f : -1.f;
    }

    if (this->amplitudeNormal.get() != 1.f)
    {
        /* scale to amplitude normal */
        arm_scale_f32(sampleBlockF32.data(), this->amplitudeNormal.get(), sampleBlockF32.data(), sampleBlockOut.size());
    }
    sampleBlockOut = sampleBlockF32;
    this->phaseInRad.set(phaseInRadSampleBlockF32.back());
}

SawF32::SawF32(F32 sampleRateInHz, F32 frequencyInHz, F32 initialPhaseInRad)
    : OscF32(sampleRateInHz, frequencyInHz, initialPhaseInRad){};

void SawF32::generate(SampleBlock<F32> &sampleBlockOut)
{
    /* get sample phases */
    Fib::Dsp::SampleBlock<F32> phaseInRadSampleBlockF32;
    arm_offset_f32(this->config.derived.getSampleBlockDeltaTimeScale().data(), this->phaseInRad.get(),
                   phaseInRadSampleBlockF32.data(), phaseInRadSampleBlockF32.size());

    /* loop phase around 2*PI and get sine values */
    Fib::Dsp::SampleBlock<F32> sampleBlockF32;
    for (std::size_t j = 0; j < phaseInRadSampleBlockF32.size(); j++)
    {
        while (phaseInRadSampleBlockF32[j] > this->phaseInRad.getUpperLimit())
        {
            phaseInRadSampleBlockF32[j] = phaseInRadSampleBlockF32[j] - this->phaseInRad.getUpperLimit();
        }
        sampleBlockF32[j] = ((phaseInRadSampleBlockF32[j] / this->phaseInRad.getUpperLimit()) * 2.f) - 1.f;
    }

    if (this->amplitudeNormal.get() != 1.f)
    {
        /* scale to amplitude normal */
        arm_scale_f32(sampleBlockF32.data(), this->amplitudeNormal.get(), sampleBlockF32.data(), sampleBlockOut.size());
    }
    sampleBlockOut = sampleBlockF32;
    this->phaseInRad.set(phaseInRadSampleBlockF32.back());
}

TriangeF32::TriangeF32(F32 sampleRateInHz, F32 frequencyInHz, F32 initialPhaseInRad)
    : OscF32(sampleRateInHz, frequencyInHz, initialPhaseInRad){};

void TriangeF32::generate(SampleBlock<F32> &sampleBlockOut)
{
    /* get sample phases */
    Fib::Dsp::SampleBlock<F32> phaseInRadSampleBlockF32;
    arm_offset_f32(this->config.derived.getSampleBlockDeltaTimeScale().data(), this->phaseInRad.get(),
                   phaseInRadSampleBlockF32.data(), phaseInRadSampleBlockF32.size());

    /* loop phase around 2*PI and get sine values */
    Fib::Dsp::SampleBlock<F32> sampleBlockF32;
    for (std::size_t j = 0; j < phaseInRadSampleBlockF32.size(); j++)
    {
        while (phaseInRadSampleBlockF32[j] > this->phaseInRad.getUpperLimit())
        {
            phaseInRadSampleBlockF32[j] = phaseInRadSampleBlockF32[j] - this->phaseInRad.getUpperLimit();
        }
        sampleBlockF32[j] = phaseInRadSampleBlockF32[j] < PI
                                ? (((phaseInRadSampleBlockF32[j] / PI) * 2.f) - 1.f)
                                : -1.f * ((((phaseInRadSampleBlockF32[j] - PI) / PI) * 2.f) - 1.f);
    }

    if (this->amplitudeNormal.get() != 1.f)
    {
        /* scale to amplitude normal */
        arm_scale_f32(sampleBlockF32.data(), this->amplitudeNormal.get(), sampleBlockF32.data(), sampleBlockOut.size());
    }
    sampleBlockOut = sampleBlockF32;
    this->phaseInRad.set(phaseInRadSampleBlockF32.back());
}

} // namespace Fib::Dsp::Waveforms