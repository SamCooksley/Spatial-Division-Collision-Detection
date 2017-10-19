#pragma once

#include "Collider.h"

class Polygon : public Collider
{
  friend class CollisionManager;
private:
  Vector2 *points;
  int size;

public:
  Polygon(const Vector2 &_position, const Vector2 &_velocity);
  ~Polygon(void);

  void Destroy(void);
  void GenerateRandom(float _minSize, float _maxSize, int _vertexCount);

  void Draw(Renderer &_renderer) override;

  bool CheckCollision(const Collider &_other, CollisionData &_data) const override;
  bool CheckCollision(const Circle   &_other, CollisionData &_data) const override;
  bool CheckCollision(const Polygon  &_other, CollisionData &_data) const override;
  bool CheckCollision(const Plane    &_other, CollisionData &_data) const override;

  Vector2 Min(void) const override;
  Vector2 Max(void) const override;

  Range MinMaxOnAxis(const Vector2 &_axis) const override;
};

