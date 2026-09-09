// Copyright (c)  2026 Lukas Neverauskis <lukas.neverauskis@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only

#pragma once

#include "dsp/Param.hpp"

namespace Fib::Dsp {
class Osc {
public:
    Osc(F32 sampleRateInHz = 44'100.f);
    virtual ~Osc() = default;

    virtual F32 get_sample(F32 phase) = 0;
    SampleBufferF32 synthesize();
    Param::Arithmetic<F32> amplitudeNormal, frequencyInHz, phaseInRad;

private:
    void getNewPhases(SampleBufferF32 &phaseSampleBufferOut);

    F32 sampleRateInHz = 44'100.f;
    F32 samplePeriodInSec;
};

namespace Waveforms {
class Sine : public Osc {
    virtual F32 get_sample(F32 phase) final { return arm_sin_f32(phase); }
};

class Square : public Osc {
    virtual F32 get_sample(F32 phase) final { return phase < PI ? 1.f : -1.f; }
};

class Saw : public Osc {
    virtual F32 get_sample(F32 phase) final {
        return ((phase / (2.f * PI)) * 2.f) - 1.f;
    }
};

class Triangle : public Osc {
    virtual F32 get_sample(F32 phase) final {
        return phase < PI ? (((phase / PI) * 2.f) - 1.f)
                          : -1.f * ((((phase - PI) / PI) * 2.f) - 1.f);
    }
};

} // namespace Waveforms
} // namespace Fib::Dsp
