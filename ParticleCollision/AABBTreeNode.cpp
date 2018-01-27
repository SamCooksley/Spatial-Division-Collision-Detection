#include "AABBTreeNode.h"

namespace AABBTree
{
	Node::Node(Node *_parent) :
		m_parent(_parent), m_item(nullptr), m_crossed(false)
	{
		m_children[0] = m_children[1] = nullptr;
	}

	Node::~Node(void)
	{
		//DestroyChildren();
	}

	void Node::SetBranch(Node *_a, Node *_b)
	{
		_a->m_parent = this;
		_b->m_parent = this;

		m_children[0] = _a;
		m_children[1] = _b;
	}

	void Node::SetLeaf(Item *_item)
	{
		DestroyChildren();

		m_item = _item;
		m_item->m_node = this;

		UpdateAABB();
	}

	bool Node::IsLeaf(void) const
	{
		return m_children[0] == nullptr;
	}

	Node *Node::GetSibling(void)
	{
		return m_parent->m_children[!GetChildIndex()];
			/*(this == m_parent->m_children[0] ?
			m_parent->m_children[1] :
			m_parent->m_children[0]);*/
	}

	int Node::GetChildIndex(void) const
	{
		return (this == m_parent->m_children[0] ? 0 : 1);
	}

	void Node::UpdateAABB(void)
	{
		if (IsLeaf())
		{
			Rect rect = m_item->GetRect();
			Vector2 offset(2.f);
      m_rect = Rect(rect.TopLeft() - offset, rect.BottomRight() + offset);
		}
		else
		{
      m_rect = Rect::Union(m_children[0]->m_rect, m_children[1]->m_rect);
		}
	}

	void Node::Draw(Renderer &_renderer)
	{
		m_rect.Draw(_renderer);
		if (IsLeaf())
		{
			m_item->GetRect().Draw(_renderer);
		}
		else
		{
			m_children[0]->Draw(_renderer);
			m_children[1]->Draw(_renderer);
		}
	}

	void Node::DestroyChildren(void)
	{
		if (!IsLeaf())
		{
			delete m_children[0];
			delete m_children[1];
			m_children[0] = m_children[1] = nullptr;
		}
	}
}