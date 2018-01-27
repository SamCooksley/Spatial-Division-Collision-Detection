#ifndef _MATH_H_
#define _MATH_H_

#include <algorithm>

#include "Vector2.h"

constexpr float PI = 3.14159265359f;
constexpr float RAD2DEG = 180.f / PI;
constexpr float DEG2RAD = PI / 180.f;

/**
 * \brief Clamps a value between a minimum and maximum
 *
 * \param [in] _value Value to clamp.
 * \param [in] _min   Minimum value.
 * \param [in] _max   Maximum value.
 * \return Returns the clamped value.
 */
inline float Clamp(float _x, float _min, float _max)
{
  return std::max(std::min(_x, _max), _min);
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
  _t = Clamp(_t, 0.f, 1.f);
  return _a + (_b - _a) * _t;
}

inline float Min(float _a, float _b)
{
  return std::min(_a, _b);
}

inline float Max(float _a, float _b)
{
  return std::max(_a, _b);
}

inline float Abs(float _x)
{
	return std::abs(_x);
}

inline float Floor(float _x)
{
  return std::floor(_x);
}

inline float Ceil(float _x)
{
  return std::ceil(_x);
}

#endif //_MATH_H_