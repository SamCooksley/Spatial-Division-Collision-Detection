#ifndef _AABBTREENODE_H_
#define _AABBTREENODE_H_

#include "Rect.h"

#include "AABBTreeItem.h"

namespace AABBTree
{
	class Node
	{
		friend class AABBTree;
	private:
		Rect m_rect;

		Node *m_parent;
		Node *m_children[2];

		Item *m_item;

		bool m_crossed;

	public:
		Node(Node *_parent);
		~Node(void);

		void SetBranch(Node *_a, Node *_b);
		void SetLeaf(Item *_item);

		bool IsLeaf(void) const;

		Node *GetSibling(void);
		int GetChildIndex(void) const;

		//Node &GetParentRefence(void);

		//void Update(void);
		void UpdateAABB(void);

		void Draw(Renderer &_renderer);

	private:
		void DestroyChildren(void);
	};
}

#endif //_AABBTREENODE_H_