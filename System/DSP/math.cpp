#include "math.hpp"

namespace Fib::Dsp::Math
{
QuadF32 secondPower(QuadF32 &input)
{
    QuadF32 result;
    arm_mult_f32(input.data(), input.data(), result.data(), result.size());
    return result;
}

QuadF32 fourthPower(QuadF32 &input)
{
    auto inputSquared = secondPower(input);
    return secondPower(inputSquared);
}
} // namespace Fib::Dsp::Math
