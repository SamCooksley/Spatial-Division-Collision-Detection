#include "Maths.h"

Vector2::Vector2(void)
{
  x = y = 0.0f;
}

Vector2::Vector2(float _val)
{
  x = y = _val;
}

Vector2::Vector2(float _x, float _y)
{
  x = _x;
  y = _y;
}

Vector2::~Vector2(void)
{
}

float Vector2::Magnitude(void) const
{
  return sqrt(x * x + y * y);
}

float Vector2::MagnitudeSq(void) const
{
  return x * x + y * y;
}

void Vector2::Normalize(void)
{
  Vector2 v = Normalized(*this);
  this->x = v.x;
  this->y = v.y;
}

Vector2 Vector2::Normalized(void) const
{
  return Normalized(*this);
}

Vector2 Vector2::Left(void) const
{
  return Vector2(y, -x);
}

Vector2 Vector2::Right(void) const
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

Vector2 Vector2::Normalized(const Vector2 &_vec)
{
  return _vec / _vec.Magnitude();
}

Vector2 Vector2::AngleToVector(float _degrees)
{
  float rad = _degrees * DEG2RAD;
  return Vector2(cos(rad), sin(rad));
}

float Vector2::VectorToAngle(Vector2 &_vector)
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