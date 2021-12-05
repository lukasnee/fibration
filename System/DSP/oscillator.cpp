#include "oscillator.hpp"

#include "arm_math.h"

namespace Fib::Dsp
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
    arm_scale_f32(this->sampleBlockDeltaTimeScale.data(), phaseDeltaInRad, this->sampleBlockDeltaTimeScale.data(),
                  this->sampleBlockDeltaTimeScale.size());
}

OscF32::OscF32(F32 sampleRateInHz, F32 frequencyInHz, F32 initialPhaseInRad)
    : config(sampleRateInHz, frequencyInHz), amplitudeNormal(1.f, 0.f, 1.f),
      frequencyInHz(0.f, 0.f, 20'000.f,
                    [&](F32 const &frequencyInHz) {
                        this->config.derived.derive(frequencyInHz);
                        return true;
                    }),
      phaseInRad(0.f, 0.f, (2.f * PI)){};

void OscF32::generateMult(SampleBlock<F32> *pSampleBlocksOut, std::size_t sampleBlocksSize)
{

    for (std::size_t i = 0; i < sampleBlocksSize; i++)
    {
        // TODO: not so sure about this optimization
        if (this->amplitudeNormal.get() != 0.f)
        {
            this->generate(pSampleBlocksOut[i]);
        }
        else
        {
            pSampleBlocksOut[i] = {0.f};
        }
    }
}

SampleBlock<F32> &OscF32::generateMult()
{
    static SampleBlock<F32> sampleBlock;
    this->generate(sampleBlock);
    return sampleBlock;
}

} // namespace Fib::Dsp
