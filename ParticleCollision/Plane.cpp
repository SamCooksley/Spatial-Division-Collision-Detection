#include "Plane.h"

#include "CollisionManager.h"

Plane::Plane(const Vector2 &_position, const Vector2 _normal, float _width) : 
	Collider(ColliderType::PLANE, _position, Vector2(0,0)),
	normal(_normal),
	m_width(_width / 2.0f)
{
  invMass = 0.0f;

	Range x = MinMaxOnAxis(Vector2(1, 0));
	Range y = MinMaxOnAxis(Vector2(0, 1));
	m_aabb.Set(x.minimum, y.minimum, x.maximum, y.maximum);
} 

Plane::~Plane(void)
{
}

void Plane::Draw(Renderer &_renderer)
{
  _renderer.SetRenderColour(0, 0, 0);
  Vector2 min(Min());
  Vector2 max(Max());
  SDL_RenderDrawLine(_renderer.Get(), min.x, min.y, max.x, max.y);
}

bool Plane::CheckCollision(const Collider &_other, CollisionData &_data) const
{
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

Vector2 Plane::Min(void) const
{
  return position + normal.Left() * m_width;
}

Vector2 Plane::Max(void) const
{
  return position + normal.Right() * m_width;
}

Range Plane::MinMaxOnAxis(const Vector2 &_axis) const
{
	Range range;
	range.minimum = Vector2::Dot(Min(), _axis);
	range.maximum = Vector2::Dot(Max(), _axis);

	if (range.minimum > range.maximum)
	{
		float tmp = range.minimum;
		range.minimum = range.maximum;
		range.maximum = tmp;
	}
  return range;
}