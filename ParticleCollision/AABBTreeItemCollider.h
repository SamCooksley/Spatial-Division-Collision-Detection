#ifndef _AABBTREEITEMCOLLIDER_H_
#define _AABBTREEITEMCOLLIDER_H_

#include "AABBTreeItem.h"

class Collider;

namespace AABBTree
{
	class ItemCollider : public Item
	{
	private:
		Collider *m_collider;

	public:
		ItemCollider(Collider &_collider);
		~ItemCollider(void);

		Vector2 GetPosition(void) const override;
		Rect GetRect(void) const override;

		void* Get(void) override { return m_collider; }
	};
}

#endif //_AABBTREEITEMCOLLIDER_H_