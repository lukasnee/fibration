#pragma once

#include "oscillator.hpp"

namespace Fib::Dsp::Waveforms
{
class SineF32 : public OscF32
{
public:
    SineF32(F32 sampleRateInHz = 44'100.f, F32 frequencyInHz = 0.f, F32 initialPhaseInRad = 0.f);
    virtual ~SineF32() = default;
    void generate(SampleBlock<F32> &sampleBlocksOut) override;
};

class SquareF32 : public OscF32
{
public:
    SquareF32(F32 sampleRateInHz = 44'100.f, F32 frequencyInHz = 0.f, F32 initialPhaseInRad = 0.f);
    virtual ~SquareF32() = default;
    void generate(SampleBlock<F32> &sampleBlocksOut) override;
};

class SawF32 : public OscF32
{
public:
    SawF32(F32 sampleRateInHz = 44'100.f, F32 frequencyInHz = 0.f, F32 initialPhaseInRad = 0.f);
    virtual ~SawF32() = default;
    void generate(SampleBlock<F32> &sampleBlocksOut) override;
};

class TriangeF32 : public OscF32
{
public:
    TriangeF32(F32 sampleRateInHz = 44'100.f, F32 frequencyInHz = 0.f, F32 initialPhaseInRad = 0.f);
    virtual ~TriangeF32() = default;
    void generate(SampleBlock<F32> &sampleBlocksOut) override;
};
} // namespace Fib::Dsp::Waveforms