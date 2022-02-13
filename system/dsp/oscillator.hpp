#pragma once

#include "rangedValue.hpp"

namespace Fib::Dsp {
class Osc {
public:
    using SynthesizeF = std::function<F32(const F32 &)>;

    struct Waveforms {
        static F32 sine(const F32 &phaseInRad) { return arm_sin_f32(phaseInRad); }
        static F32 square(const F32 &phaseInRad) { return phaseInRad < PI ? 1.f : -1.f; }
        static F32 saw(const F32 &phaseInRad) { return ((phaseInRad / (2.f * PI)) * 2.f) - 1.f; }
        static F32 triangle(const F32 &phaseInRad) {
            return phaseInRad < PI ? (((phaseInRad / PI) * 2.f) - 1.f) : -1.f * ((((phaseInRad - PI) / PI) * 2.f) - 1.f);
        }
    };

    Osc(SynthesizeF synthesizeF, F32 sampleRateInHz = 44'100.f);
    virtual ~Osc() = default;

    SampleBufferF32 synthesize();
    SynthesizeF synthesizeF;
    RangedValue<F32> amplitudeNormal, frequencyInHz, phaseInRad;

private:
    void getNewPhases(SampleBufferF32 &phaseSampleBufferOut);

    F32 sampleRateInHz = 44'100.f;
    F32 samplePeriodInSec, phaseDeltaInRad;
};

} // namespace Fib::Dsp
