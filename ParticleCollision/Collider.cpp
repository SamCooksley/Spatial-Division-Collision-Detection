#include "Collider.h"
#include "Plane.h"
#include "CollisionManager.h"

Collider::Collider(ColliderType _type, const Vector2 &_position, const Vector2 &_velocity) : 
  m_position(_position), m_velocity(_velocity)
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

const Vector2& Collider::GetPosition() const
{
  return m_position;
}

const Vector2& Collider::GetVelocity() const
{
  return m_velocity;
}

const Rect& Collider::GetAABB() const
{
  return m_aabb;
}

void Collider::DrawRect(Renderer& _renderer) const
{
  m_aabb.Draw(_renderer);
}