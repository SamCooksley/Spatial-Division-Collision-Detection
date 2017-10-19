#include "Collider.h"
#include "Plane.h"
#include "CollisionManager.h"

Collider::Collider(ColliderType _type, const Vector2 &_position, const Vector2 &_velocity) : 
	position(_position), velocity(_velocity), m_item(*this), m_aabbItem(*this)
{
  type = _type;

  bounciness = 1;
}

Collider::~Collider(void)
{
}

void Collider::Update(float _deltaTime)
{
  //velocity.y += 100.0f * _deltaTime;
  position += velocity * _deltaTime;

	Range x =MinMaxOnAxis(Vector2(1, 0));
	Range y = MinMaxOnAxis(Vector2(0, 1));
	m_aabb.Set(x.minimum, y.minimum, x.maximum, y.maximum);
}

QuadTree::Item *Collider::AsItem(void) const
{
	return (QuadTree::Item*)&m_item;
}

AABBTree::Item *Collider::AsAABBItem(void) const
{
	return (AABBTree::Item*)&m_aabbItem;
}