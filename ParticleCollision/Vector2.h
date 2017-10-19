#ifndef _VECTOR2_H_
#define _VECTOR2_H_

struct Vector2
{
public:
  float x;
  float y;

  Vector2(void);
  Vector2(float _val);
  Vector2(float _x, float _y);

  ~Vector2(void);

  float Magnitude(void) const;
  float MagnitudeSq(void) const;

  void Normalize(void);
  Vector2 Normalized(void) const;

  Vector2 Left(void) const;
  Vector2 Right(void) const;

  Vector2 operator=(const Vector2 &_rhs);

  Vector2 operator+=(const Vector2 &_rhs);
  Vector2 operator-=(const Vector2 &_rhs);

  Vector2 operator*=(const float &_scalar);
  Vector2 operator/=(const float &_scalar);

  static Vector2 Normalized(const Vector2 &_vec);

  static float Dot(const Vector2 &_lhs, const Vector2 &_rhs);

  static Vector2 AngleToVector(float _degrees);
  static float VectorToAngle(Vector2 &_vector);
};

Vector2 operator+(const Vector2 &_lhs, const Vector2 &_rhs);
Vector2 operator-(const Vector2 &_lhs, const Vector2 &_rhs);

Vector2 operator*(const Vector2 &_lhs, const float &_rhs);
Vector2 operator/(const Vector2 &_lhs, const float &_rhs);

#endif //_VECTOR2_H_