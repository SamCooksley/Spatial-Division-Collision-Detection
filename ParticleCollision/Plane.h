#pragma once

#include "Maths.h"
#include "Collider.h"

class Plane : public Collider
{
  friend class CollisionManager;
private:
  Vector2 normal;
	float m_width;

public:
  Plane(const Vector2 &_position, const Vector2 _normal, float _width);
  ~Plane(void);

  void Draw(Renderer &_renderer) override;

  bool CheckCollision(const Collider &_other, CollisionData &_data) const override;
  bool CheckCollision(const Circle   &_other, CollisionData &_data) const override;
  bool CheckCollision(const Polygon  &_other, CollisionData &_data) const override;
  bool CheckCollision(const Plane    &_other, CollisionData &_data) const override;

  Vector2 Min(void) const override;
  Vector2 Max(void) const override;

  Range MinMaxOnAxis(const Vector2 &_axis) const override;
};