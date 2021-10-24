#pragma once

#include "map.hpp"
namespace Fib::DSP::Osc
{
    class OscF32
    {
    public:
        struct Config
        {
            Config(F32 sampleRateInHz, F32 frequencyInHz);

            F32 sampleRateInHz = 44'100.f;
            struct Derived
            {
            public:
                Derived(F32 sampleRateInHz = 44'100.f, F32 frequencyInHz = 0.f);
                void derive(F32 frequencyInHz);
                const F32 &getSamplePeriodInSec() const { return this->samplePeriodInSec; }
                const F32 &getPhaseDeltaInRad() const { return this->phaseDeltaInRad; }
                /* I want to return CONST reference but the flimsy CMSIS DSP API doesn't let me :') */
                SampleBlock<F32> &getSampleBlockDeltaTimeScale() { return this->sampleBlockDeltaTimeScale; }

            private:
                F32 samplePeriodInSec, phaseDeltaInRad;
                SampleBlock<F32> sampleBlockDeltaTimeScale;
            } derived;
        } config;

        OscF32(F32 sampleRateInHz = 44'100.f, F32 frequencyInHz = 0.f, F32 initialPhaseInRad = 0.f);
        virtual ~OscF32() = default;

        virtual void generate(SampleBlock<F32> &sampleBlocksOut) = 0;

        void generateMult(SampleBlock<F32> *pSampleBlocksOut, std::size_t sampleBlocksSize);

        RangedValue<F32> amplitudeNormal, frequencyInHz, phaseInRad;
    };

    class SineF32 : public OscF32
    {
    public:
        SineF32(F32 sampleRateInHz = 44'100.f, F32 frequencyInHz = 0.f, F32 initialPhaseInRad = 0.f);
        virtual ~SineF32() = default;
        void generate(SampleBlock<F32> &sampleBlocksOut) override;
    };

} // Fib::DSP::Osc
