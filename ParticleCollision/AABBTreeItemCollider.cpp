#include "AABBTreeItemCollider.h"

#include "Collider.h"

namespace AABBTree
{
	ItemCollider::ItemCollider(Collider &_collider) :
		m_collider(&_collider)
	{
	}

	ItemCollider::~ItemCollider(void)
	{
	}

	Vector2 ItemCollider::GetPosition(void) const
	{
		return m_collider->Position();
	}

	Rect ItemCollider::GetRect(void) const
	{
		return m_collider->AABB();
	}
}
