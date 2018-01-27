#include "Maths.h"

Vector2::Vector2() :
  x(.0f), y(.0f)
{ }

Vector2::Vector2(float _val) :
  x(_val), y(_val)
{ }

Vector2::Vector2(float _x, float _y) :
  x(_x), y(_y)
{ }

Vector2::Vector2(const Vector2& _other) :
  x(_other.x), y(_other.y)
{ }

float Vector2::Magnitude() const
{
  return sqrt(abs(x * x + y * y));
}

float Vector2::MagnitudeSq() const
{
  return x * x + y * y;
}

Vector2 Vector2::Normalized() const
{
  float m = Magnitude();
  if (m == .0f)
  {
    throw;
  }
  return *this * (1.f / m);
}

Vector2 Vector2::Left() const
{
  return Vector2(y, -x);
}

Vector2 Vector2::Right() const
{
  return Vector2(-y, x);
}

float Vector2::Dot(const Vector2 &_lhs, const Vector2 &_rhs)
{
  return _lhs.x * _rhs.x + _lhs.y * _rhs.y;
}

Vector2 Vector2::operator=(const Vector2 &_rhs)
{
  this->x = _rhs.x;
  this->y = _rhs.y;

  return *this;
}

Vector2 Vector2::operator+=(const Vector2 &_rhs)
{
  *this = *this + _rhs;
  return *this;
}

Vector2 Vector2::operator-=(const Vector2 &_rhs)
{
  *this = *this - _rhs;
  return *this;
}

Vector2 Vector2::operator*=(const float &_scalar)
{
  *this = *this * _scalar;
  return *this;
}

Vector2 Vector2::operator/=(const float &_scalar)
{
  *this = *this / _scalar;
  return *this;
}

Vector2 Vector2::AngleToVector(float _radians)
{
  return Vector2(cos(_radians), sin(_radians));
}

float Vector2::VectorToAngle(const Vector2 &_vector)
{
  return atan2(_vector.y, _vector.x);
}

Vector2 operator+(const Vector2 &_lhs, const Vector2 &_rhs)
{
  return Vector2(_lhs.x + _rhs.x, _lhs.y + _rhs.y);
}

Vector2 operator-(const Vector2 &_lhs, const Vector2 &_rhs)
{
  return Vector2(_lhs.x - _rhs.x, _lhs.y - _rhs.y);
}

Vector2 operator*(const Vector2 &_lhs, const float &_rhs)
{
  return Vector2(_lhs.x * _rhs, _lhs.y * _rhs);
}

Vector2 operator/(const Vector2 &_lhs, const float &_rhs)
{
  return Vector2(_lhs.x / _rhs, _lhs.y / _rhs);
}