#pragma once

#include <cstdint>
#include <limits>
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
         * @param bitDepth word width in bits from 1 to 32 !
         * @return constexpr std::uint32_t max value
         * 
         * @note 1-32 bitDepth max !
         */
        template <std::uint32_t bitDepth>
        constexpr std::uint32_t bitDepthToMaxValue()
        {
            return (std::numeric_limits<std::uint32_t>::max() >> (32 - bitDepth));
        }

        /**
         * @brief get the center value for the given bit depth
         * 
         * @param bitDepth 
         * @return constexpr std::uint32_t 
         */
        template <std::uint64_t bitDepth>
        constexpr std::uint64_t bitDepthToCenterValue()
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
         * @return T mapped (scaled) output value
         */
        template <typename T>
        T map(const T &in,
              const T &inMin,
              const T &inMax,
              const T &outMin,
              const T &outMax)
        {
            return (in - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
        }

        /**
         * @brief map (scale) value from [0 inMax] absolute range to [0 outMax] absolute range
         * 
         * @param in input value to scale
         * @param inMax input max value, the upper limit
         * @param outMax output max value, the upper limit
         * @return T mapped (scaled) output value
         */
        template <typename T>
        T map(const T &in,
              const T &inMax,
              const T &outMax)
        {
            return map(in, static_cast<T>(0), inMax, static_cast<T>(0), outMax);
        }

        constexpr q31_t floatToQ31(float in)
        {
            return (in > 0.99999997f ? std::numeric_limits<q31_t>::max()
                    : in < -1.0f
                        ? std::numeric_limits<q31_t>::min()
                        : static_cast<q31_t>(
                              in *
                              (static_cast<float>(std::numeric_limits<q31_t>::max()))));
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

            if (sample < bitDepthToCenterValue<32>())
            {
                result = static_cast<q31_t>(sample) - bitDepthToCenterValue<32>();
            }
            else
            {
                result = static_cast<q31_t>(sample - bitDepthToCenterValue<32>());
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

                result = u32ToQ31(map<std::uint32_t>(sample, bitDepthToMaxValue<sampleBitDepth>(), bitDepthToMaxValue<32>()));

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
                result = static_cast<std::uint32_t>(q31 + bitDepthToCenterValue<32>());
            }
            else
            {
                result = static_cast<std::uint32_t>(q31) + bitDepthToCenterValue<32>();
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

            result = map<std::uint32_t>(q31ToU32(q31), bitDepthToMaxValue<32>(), bitDepthToMaxValue<sampleBitDepth>());

            return result;
        }

        /**
         * @brief swap uint32 endianness
         * 
         * @param val 
         * @return std::uint32_t 
         */
        std::uint32_t swap(std::uint32_t val)
        {
            val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
            return (val << 16) | (val >> 16);
        }

        template <typename T>
        class Range
        {
        public:
            constexpr Range(T const &lower, T const &upper) : upper(upper),lower(lower) {}
            bool isInRange(T value) const
            {
                return (this->lower < value && value < this->upper);
            };
            constexpr T getUpper() const { return this->upper; }
            constexpr T getLower() const { return this->lower; }

        private:
            const T upper;
            const T lower;
        };
    } // namespace DSP
} // namespace FibUtil