#include "math.hpp"

namespace Fib::Dsp::Math
{
QuadF32 secondPower(QuadF32 &bases)
{
    QuadF32 result;
    arm_mult_f32(bases.data(), bases.data(), result.data(), result.size());
    return result;
}

QuadF32 fourthPower(QuadF32 input)
{
    return secondPower(secondPower(input));
}
} // namespace Fib::Dsp::Math
