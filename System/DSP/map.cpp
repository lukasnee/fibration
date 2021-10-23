#include "map.hpp"

namespace Fib
{
    namespace DSP
    {
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

        std::uint32_t swap(std::uint32_t val)
        {
            val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
            return (val << 16) | (val >> 16);
        }

    } // namespace DSP
} // namespace Fib