
/** @brief Feed new value into parametrized Exponential Moving Average (EMA) filter. Compared to a Simple Moving Average (SMA)
 * filter, this one requires no buffer, thus more RAM efficient. For more info see: https://en.wikipedia.org/wiki/Moving_average
 *
 * @attention On first iteration, argument for `previousEmaInDeg` should be the same as `newValue`. On every other iteration
 * `previousEmaInDeg` should be feed with previous iteration result.
 *
 * @param alphaCoeff [in] Exponential moving average weighting coefficient.
 * @param previousEmaInDeg [in] Previous EMA value in angular degrees.
 * @param newValue [in] New (current) error control value in degrees.
 *
 * @return New EMA filtered value result.
 **/
static AttitudeFloat expMovingAverageUpdate(const AttitudeFloat &alphaCoeff, const AttitudeFloat &previousEmaInDeg,
                                            const AttitudeFloat &newValue)
{
  return (alphaCoeff * newValue + (1.0 - alphaCoeff) * previousEmaInDeg);
}

/** @brief Feed a new value into parametrized EMA filter where `alpha` (weighting coeffcient) is resolved from following
 * relation: EMA_alpha = 2/(N_SMA + 1). This is because weights of a Simple Moving Average (SMA) and EMA filters have the
 * same "center of mass" when EMA_alpha = 2/(N_SMA + 1). See: https://en.wikipedia.org/wiki/Moving_average
 *
 * @attention On first iteration, argument for `previousEmaInDeg` should be the same as `controlErrorInDeg`. And every other
 * iteration `previousEmaInDeg` should be feed with previous iteration result.
 *
 * @param movingAverageWindowInSamples [in] Time window expressed in samples (if it were an equivalent single moving average filter).
 * @param previousEmaInDeg [in] Previous EMA value in angular degrees.
 * @param newValue [in] New (current) error control value in degrees.
 *
 * @return New EMA filtered value.
 **/
static AttitudeFloat emaUpdateFromSampleWindowApprox(const AttitudeFloat &movingAverageWindowInSamples,
                                                     const AttitudeFloat &previousEmaInDeg, const AttitudeFloat &controlErrorInDeg)
{
  const AttitudeFloat emaAlpha = 2.0 / (movingAverageWindowInSamples + 1.0);

  return expMovingAverageUpdate(emaAlpha, previousEmaInDeg, controlErrorInDeg);
}
