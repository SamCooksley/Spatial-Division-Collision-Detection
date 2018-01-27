#ifndef _VECTOR2_H_
#define _VECTOR2_H_

class Vector2
{
public:
  float x, y;

  Vector2();
  Vector2(float _val);
  Vector2(float _x, float _y);
  Vector2(const Vector2& _other);

  float Magnitude() const;
  float MagnitudeSq() const;

  Vector2 Normalized() const;

  Vector2 Left() const;
  Vector2 Right() const;

  Vector2 operator=(const Vector2 &_rhs);

  Vector2 operator+=(const Vector2 &_rhs);
  Vector2 operator-=(const Vector2 &_rhs);

  Vector2 operator*=(const float &_scalar);
  Vector2 operator/=(const float &_scalar);

  static float Dot(const Vector2 &_lhs, const Vector2 &_rhs);

  static Vector2 AngleToVector(float _radians);
  static float VectorToAngle(const Vector2 &_vector);
};

Vector2 operator+(const Vector2 &_lhs, const Vector2 &_rhs);
Vector2 operator-(const Vector2 &_lhs, const Vector2 &_rhs);

Vector2 operator*(const Vector2 &_lhs, const float &_rhs);
Vector2 operator/(const Vector2 &_lhs, const float &_rhs);

#endif //_VECTOR2_H_