#include "Plane.h"

#include "CollisionManager.h"

Plane::Plane(const Vector2 &_position, const Vector2& _normal, float _width) : 
	Collider(ColliderType::PLANE, _position, Vector2(0,0)),
  m_normal(_normal),
	m_width(_width * .5f)
{
  invMass = 0.f;

	Range x = MinMaxOnAxis(Vector2(1, 0));
	Range y = MinMaxOnAxis(Vector2(0, 1));
	m_aabb = Rect(x.min, y.min, x.max, y.max);
} 

void Plane::Draw(Renderer &_renderer)
{
  Vector2 min(Min()), max(Max());
  SDL_RenderDrawLine(_renderer.Get(), min.x, min.y, max.x, max.y);
}

bool Plane::CheckCollision(const Collider &_other, CollisionData &_data) const
{
  //go through the other collider with this object now specified.
  return _other.CheckCollision(*this, _data);
}

bool Plane::CheckCollision(const Circle &_other, CollisionData &_data) const
{
  return CollisionManager::CheckCollision(_other, *this, _data);
}

bool Plane::CheckCollision(const Polygon &_other, CollisionData &_data) const
{
  return CollisionManager::CheckCollision(_other, *this, _data);
}

bool Plane::CheckCollision(const Plane &_other, CollisionData &_data) const 
{
  return false;
}

Vector2 Plane::Min() const
{
  //TODO: this may not be the minimum point.
  return position + m_normal.Left() * m_width;
}

Vector2 Plane::Max() const
{
  //TODO: this may not be the maximum point.
  return position + m_normal.Right() * m_width;
}

Range Plane::MinMaxOnAxis(const Vector2 &_axis) const
{
  //Project the minimum and maximum of the plane on the axis.
	Range range;
	range.min = Vector2::Dot(Min(), _axis);
	range.max = Vector2::Dot(Max(), _axis);
  range.Sort();
  return range;
}