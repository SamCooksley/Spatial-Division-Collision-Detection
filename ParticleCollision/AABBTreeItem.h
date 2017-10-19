#ifndef _AABBTREEITEM_H_
#define _AABBTREEITEM_H_

#include "Rect.h"

namespace AABBTree
{
	class Node;

	class Item
	{
		friend class Node;
	private: 
		Node *m_node;

	public:
		Item();
		virtual ~Item();

		virtual Vector2 GetPosition(void) const = 0;
		virtual Rect GetRect(void) const = 0;

		virtual void *Get(void) = 0;

		void Draw(Renderer &_renderer);
	};
}

#endif //_AABBTREEITEM_H_