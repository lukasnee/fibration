#include <cstdint>
#include "stm32f303xe.h"
extern "C"
{
#include "arm_math.h"
}

namespace Fib
{
    namespace DSP
    {
        /**
         * @brief get the max value for the given bit depth
         * 
         * @param bitDepth word width in bits, i.e.: 32 to get 32-bit max value
         * @return constexpr std::uint32_t max value
         */
        template <std::uint32_t bitDepth>
        constexpr std::uint32_t bitDepthToMaxValue()
        {
            return ((bitDepth << 1) - 1);
        }

        /**
         * @brief get the center value for the given bit depth
         * 
         * @param bitDepth 
         * @return constexpr std::uint32_t 
         */
        template <std::uint32_t bitDepth>
        constexpr std::uint32_t bitDepthToCenterValue()
        {
            return (bitDepthToMaxValue<bitDepth>() / 2);
        }

        /**
         * @brief  map (scale) value from [inMin inMax] relative range to [outMin outMax] relative range
         * 
         * @param in input value to scale
         * @param inMin input min value, the lower limit
         * @param inMax input max value, the upper limit
         * @param outMin output min value, the lower limit
         * @param outMax output max value, the upper limit
         * @return std::uint32_t mapped (scaled) output value
         */
        std::uint32_t map(const std::uint32_t &in,
                          const std::uint32_t &inMin,
                          const std::uint32_t &inMax,
                          const std::uint32_t &outMin,
                          const std::uint32_t &outMax)
        {
            return (in - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
        }

        /**
         * @brief map (scale) value from [0 inMax] absolute range to [0 outMax] absolute range
         * 
         * @param in input value to scale
         * @param inMax input max value, the upper limit
         * @param outMax output max value, the upper limit
         * @return std::uint32_t mapped (scaled) output value
         */
        std::uint32_t map(const std::uint32_t &in,
                          const std::uint32_t &inMax,
                          const std::uint32_t &outMax)
        {
            return map(in, 0, inMax, 0, outMax);
        }

        /**
         * @brief 
         * 
         * @param sample 
         * @return q31_t 
         */
        q31_t u32ToQ31(const std::uint32_t &sample)
        {
            q31_t result;

            if (sample < bitDepthToCenterValue<32UL>())
            {
                result = static_cast<q31_t>(sample) - bitDepthToCenterValue<32UL>();
            }
            else
            {
                result = static_cast<q31_t>(sample - bitDepthToCenterValue<32UL>());
            }

            return result;
        }

        /**
         * @brief 
         * 
         * @tparam sampleBitDepth 
         * @param sample 
         * @return q31_t 
         */
        template <std::uint32_t sampleBitDepth>
        q31_t sampleToQ31(const std::uint32_t &sample)
        {
            {
                q31_t result;

                u32ToQ31(map(sample, bitDepthToMaxValue<sampleBitDepth>(), bitDepthToMaxValue<32UL>()));

                return result;
            }
        }

        /**
         * @brief 
         * 
         * @param q31 
         * @return std::uint32_t 
         */
        std::uint32_t q31ToU32(const q31_t &q31)
        {
            std::uint32_t result;

            if (q31 < 0)
            {
                result = static_cast<std::uint32_t>(q31 + bitDepthToCenterValue<32UL>());
            }
            else
            {
                result = static_cast<std::uint32_t>(q31) + bitDepthToCenterValue<32UL>();
            }

            return result;
        }

        /**
         * @brief 
         * 
         * @tparam sampleBitDepth 
         * @param q31 
         * @return std::uint32_t 
         */
        template <std::uint32_t sampleBitDepth>
        std::uint32_t q31ToSample(const q31_t &q31)
        {
            std::uint32_t result;

            result = map(q31ToU32(q31), bitDepthToMaxValue<32UL>(), bitDepthToMaxValue<sampleBitDepth>());

            return result;
        }

    } // namespace DSP
} // namespace FibUtil