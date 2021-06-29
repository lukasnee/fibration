// #include "map.hpp"
#include <limits>

namespace Fib
{
    namespace DSP
    {
        // template <std::uint32_t bitDepth>
        // constexpr std::uint32_t bitDepthToMaxValue()
        // {
        //     return ((bitDepth << 1) - 1);
        // }

        // template <std::uint32_t bitDepth>
        // constexpr std::uint32_t bitDepthToCenterValue()
        // {
        //     return (bitDepthToMaxValue<bitDepth>() / 2);
        // }

        // std::uint32_t map(const std::uint32_t &in,
        //                   const std::uint32_t &inMin,
        //                   const std::uint32_t &inMax,
        //                   const std::uint32_t &outMin,
        //                   const std::uint32_t &outMax)
        // {
        //     return (in - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
        // }

        // std::uint32_t map(const std::uint32_t &in,
        //                   const std::uint32_t &inMax,
        //                   const std::uint32_t &outMax)
        // {
        //     return map(in, 0, inMax, 0, outMax);
        // }

        // q31_t u32ToQ31(const std::uint32_t &sample)
        // {
        //     q31_t result;

        //     if (sample < bitDepthToCenterValue<32UL>())
        //     {
        //         result = static_cast<q31_t>(sample) - bitDepthToCenterValue<32UL>();
        //     }
        //     else
        //     {
        //         result = static_cast<q31_t>(sample - bitDepthToCenterValue<32UL>());
        //     }

        //     return result;
        // }

        // template <std::uint32_t sampleBitDepth>
        // q31_t sampleToQ31(const std::uint32_t &sample)
        // {
        //     q31_t result;

        //     u32ToQ31(map(sample, bitDepthToMaxValue<sampleBitDepth>(), bitDepthToMaxValue<32UL>()));

        //     return result;
        // }

        // std::uint32_t q31ToU32(const q31_t &q31)
        // {
        //     std::uint32_t result;

        //     if (q31 < 0)
        //     {
        //         result = static_cast<std::uint32_t>(q31 + bitDepthToCenterValue<32UL>());
        //     }
        //     else
        //     {
        //         result = static_cast<std::uint32_t>(q31) + bitDepthToCenterValue<32UL>();
        //     }

        //     return result;
        // }

        // template <std::uint32_t sampleBitDepth>
        // std::uint32_t q31ToSample(const q31_t &q31)
        // {
        //     std::uint32_t result;

        //     result = map(q31ToU32(q31), bitDepthToMaxValue<32UL>(), bitDepthToMaxValue<sampleBitDepth>());

        //     return result;
        // }

    } // namespace DSP
} // namespace FibUtil