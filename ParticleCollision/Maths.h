#ifndef _MATH_H_
#define _MATH_H_

#include <algorithm>

#include "Vector2.h"

const float PI = 3.14159f;
const float RAD2DEG = 180.0f / PI;
const float DEG2RAD = PI / 180.0f; 

/**
 * \brief Clamps a value between a minimum and maximum
 *
 * \param [in] _value Value to clamp.
 * \param [in] _min   Minimum value.
 * \param [in] _max   Maximum value.
 * \return Returns the clamped value.
 */
inline float Clamp(float _value, float _min, float _max)
{
  return std::max(std::min(_value, _max), _min);
}

/**
 * \brief Linearly interpolates between a and b.
 * 
 * \param [in] _a Start value.
 * \param [in] _b End value.
 * \param [in] _t Interpolation value.
 * \return Returns the interplated value between a and b.
 */
inline float Lerp(float _a, float _b, float _t)
{
  _t = Clamp(_t, 0.0f, 1.0f);
  return _a + (_b - _a) * _t;
}

inline float Max(float _a, float _b)
{
  if (_b > _a) { return _b; }
  return _a;
}

inline float Min(float _a, float _b)
{
  if (_b < _a) { return _b; }
  return _a;
}

inline float Abs(float _value)
{
	return std::abs(_value);
}

#endif //_MATH_H_