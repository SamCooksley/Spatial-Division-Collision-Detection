#include "Circle.h"

#include "SDL.h"
#include "CollisionManager.h"

Circle::Circle(const Vector2 &_position, const Vector2 &_velocity, float _radius) : Collider(ColliderType::CIRCLE, _position, _velocity)
{
  radius = _radius;
  float area = PI * (radius * radius);
  invMass = 1.0f / area;

	Range x = MinMaxOnAxis(Vector2(1, 0));
	Range y = MinMaxOnAxis(Vector2(0, 1));
	m_aabb = Rect(x.min, y.min, x.max, y.max);
}

Circle::~Circle()
{
}

void Circle::Draw(Renderer &_renderer)
{
  SDL::DrawCircle(_renderer, position.x, position.y, radius);
}

bool Circle::CheckCollision(const Collider &_other, CollisionData &_data) const 
{
  return _other.CheckCollision(*this, _data);
}

bool Circle::CheckCollision(const Circle &_other, CollisionData &_data) const 
{
  return CollisionManager::CheckCollision(*this, _other, _data);
}

bool Circle::CheckCollision(const Polygon &_other, CollisionData &_data) const 
{
  return CollisionManager::CheckCollision(*this, _other, _data);
}

bool Circle::CheckCollision(const Plane &_other, CollisionData &_data) const
{
  return CollisionManager::CheckCollision(*this, _other, _data);
}

Vector2 Circle::Min(void) const
{
  return position - Vector2(radius);
}

Vector2 Circle::Max(void) const
{
  return position + Vector2(radius);
}

Range Circle::MinMaxOnAxis(const Vector2 &_axis) const
{
  float pos = Vector2::Dot(position, _axis);
  return Range(pos - radius, pos + radius);
}