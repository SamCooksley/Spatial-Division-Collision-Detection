#include "Circle.h"

#include "SDL.h"
#include "CollisionManager.h"

Circle::Circle(const Vector2& _position, const Vector2& _velocity, float _radius) : Collider(ColliderType::CIRCLE, _position, _velocity)
{
  m_radius = _radius;
  float area = PI * (m_radius * m_radius);
  m_invMass = 1.f / area;

	Range x = MinMaxOnAxis(Vector2(1, 0));
	Range y = MinMaxOnAxis(Vector2(0, 1));
	m_aabb = Rect(x.min, y.min, x.max, y.max);
}

void Circle::Draw(Renderer& _renderer)
{
  SDL::DrawCircle(_renderer, m_position.x, m_position.y, m_radius);
}

bool Circle::CheckCollision(const Collider& _other, CollisionData& _data) const 
{
  return _other.CheckCollision(*this, _data);
}

bool Circle::CheckCollision(const Circle& _other, CollisionData& _data) const 
{
  return CollisionManager::CheckCollision(*this, _other, _data);
}

bool Circle::CheckCollision(const Polygon& _other, CollisionData& _data) const 
{
  return CollisionManager::CheckCollision(*this, _other, _data);
}

bool Circle::CheckCollision(const Plane& _other, CollisionData& _data) const
{
  return CollisionManager::CheckCollision(*this, _other, _data);
}

Range Circle::MinMaxOnAxis(const Vector2& _axis) const
{
  float pos = Vector2::Dot(m_position, _axis);
  return Range(pos - m_radius, pos + m_radius);
}