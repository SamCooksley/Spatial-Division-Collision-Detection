#include "Collider.h"
#include "Plane.h"
#include "CollisionManager.h"

Collider::Collider(ColliderType _type, const Vector2 &_position, const Vector2 &_velocity) : 
  m_position(_position), m_velocity(_velocity), m_quadItem(*this), m_aabbItem(*this)
{
  m_type = _type;

  m_bounciness = 1.f;
}

Collider::~Collider()
{ }

void Collider::Update(float _deltaTime)
{
  //velocity.y += 100.0f * _deltaTime;
  m_position += m_velocity * _deltaTime;

	Range x = MinMaxOnAxis(Vector2(1.f, 0.f));
	Range y = MinMaxOnAxis(Vector2(0.f, 1.f));

	m_aabb = Rect(x.min, y.min, x.max, y.max);
}

void Collider::Draw(Renderer& _renderer)
{ }

const Vector2& Collider::Position() const
{
  return m_position;
}

const Vector2& Collider::Velocity() const
{
  return m_velocity;
}

const Rect& Collider::AABB() const
{
  return m_aabb;
}

QuadTree::Item* Collider::AsQuadItem() const
{
	return static_cast<QuadTree::Item*>(&m_quadItem);
}

AABBTree::Item* Collider::AsAABBItem() const
{
	return static_cast<AABBTree::Item*>(&m_aabbItem);
}