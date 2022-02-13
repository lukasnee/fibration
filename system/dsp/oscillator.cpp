#include "oscillator.hpp"

#include "arm_math.h"

namespace Fib::Dsp {

Osc::Osc(SynthesizeF synthesizeF, F32 sampleRateInHz)
    : synthesizeF(synthesizeF), amplitudeNormal(1.f, 0.f, 1.f),
      frequencyInHz(0.f, 0.f, 20'000.f,
                    [&](F32 const &frequencyInHz) { this->phaseDeltaInRad = frequencyInHz * this->samplePeriodInSec; }),
      phaseInRad(0.f, 0.f, (2.f * PI)), sampleRateInHz(sampleRateInHz) {
    this->samplePeriodInSec = 2 * PI / this->sampleRateInHz;
};

SampleBufferF32 Osc::synthesize() {
    SampleBufferF32 sampleBlockF32;
    Fib::Dsp::SampleBufferF32 newPhasesInRad;
    this->getNewPhases(newPhasesInRad);
    for (std::size_t i = 0; i < sampleBlockF32.size(); i++) {
        sampleBlockF32[i] = this->synthesizeF(newPhasesInRad[i]);
    }
    /* scale to amplitude normal */
    if (this->amplitudeNormal.get() != 1.f) {
        arm_scale_f32(sampleBlockF32.data(), this->amplitudeNormal.get(), sampleBlockF32.data(), sampleBlockF32.size());
    }
    return sampleBlockF32;
}

void Osc::getNewPhases(SampleBufferF32 &phaseSampleBufferOut) {
    // TODO: try optimizing with CMSIS math
    for (auto &phaseSample : phaseSampleBufferOut) {
        phaseSample = this->phaseInRad.get() + this->phaseDeltaInRad;
        if (phaseSample > this->phaseInRad.upperLimit()) {
            phaseSample = phaseSample - this->phaseInRad.upperLimit();
        }
        this->phaseInRad.set(phaseSample);
    }
    // arm_offset_f32(this->phaseTable.data(), this->phaseInRad.get(), result.data(), result.size());
}
} // namespace Fib::Dsp
