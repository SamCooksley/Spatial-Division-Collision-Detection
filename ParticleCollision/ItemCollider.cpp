#include "ItemCollider.h"

#include "Collider.h"

namespace QuadTree
{
  ItemCollider::ItemCollider(Collider& _collider) :
		m_collider(&_collider)
  { }

  ItemCollider::~ItemCollider()
  { }

  Vector2 ItemCollider::GetPosition() const
  {
    return m_collider->Position();
  }

  Rect ItemCollider::GetRect() const
  {
		return m_collider->AABB();
  }
}
