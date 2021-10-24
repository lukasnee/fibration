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

            struct Ranges
            {
                static constexpr auto amplitudeNormal = Range<F32>(0.f, 1.f);
                static constexpr auto frequencyInHz = Range<F32>(0.f, 20'000.f);
                static constexpr auto phaseInRad = Range<F32>(0.f, (2.f * PI));
            };

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

        F32 getAmplitudeNormal();
        bool setAmplitudeNormal(F32 amplitudeNormal);

        F32 getFrequencyInHz();
        bool setFrequencyInHz(F32 frequencyInHz);

        F32 getPhaseInRad();
        bool setPhaseInRad(F32 phaseInRad);

    private:
        F32 amplitudeNormal = 0.f, frequencyInHz = 0.f, phaseInRad = 0.f;
    };

    class SineF32 : public OscF32
    {
    public:
        SineF32(F32 sampleRateInHz = 44'100.f, F32 frequencyInHz = 0.f, F32 initialPhaseInRad = 0.f);
        virtual ~SineF32() = default;
        void generate(SampleBlock<F32> &sampleBlocksOut) override;
    };

} // Fib::DSP::Osc
