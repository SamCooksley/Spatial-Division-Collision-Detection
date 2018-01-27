#include "QuadTreeNode.h"

namespace QuadTree
{
	Node::Node(Node *_parent, const Rect &_rect, int _maxItems, int _depth) :
		m_parent(_parent), m_rect(_rect), m_divided(false), m_depth(_depth)
  {
    SetMaxItems(_maxItems);
  }

  Node::~Node(void)
  {
		Destroy();
  }

	void Node::Destroy(void)
	{
		RemoveDivide(); //destroy children
		Clear();
	}

	void Node::Clear(void)
	{
		m_items.clear();
		if (m_divided)
		{
			for (auto& child : m_children)
			{
				child->Clear();
			}
		}
	}

  void Node::SetMaxItems(int _max)
  {
    m_maxItems = _max;
		m_items.reserve(m_maxItems + 1);
  }

	void Node::Insert(Item *_item)
	{
		if (!InsertInChildren(_item))
		{			
			m_items.emplace_back(_item);
			
			if (m_depth < 10 && m_items.size() >= m_maxItems)
			{
				Divide();
			}
		}
	}

	int Node::GetItemCount(void) const
	{
		int count = m_items.size();

		if (m_divided)
		{
			for (auto& child : m_children)
			{
				count += child->GetItemCount();
			}
		}

		return count;
	}

	void Node::Draw(Renderer &_renderer)
	{
		m_rect.Draw(_renderer);

		if (m_divided)
		{
			for (auto& child : m_children)
			{
				child->Draw(_renderer);
			}
		}

		for (auto& item : m_items)
		{
			item->Draw(_renderer);
		}
	}

	void Node::Divide(void)
	{
		if (m_divided) { return; }

		float width = m_rect.Width() / s_xDivisions;
		float height = m_rect.Height() / s_yDivisions;

		Rect rect;

		Vector2 offset = m_rect.min;

		for (int y = 0; y < s_yDivisions; y++)
		{
			rect.min.y = offset.y + y * height;
			rect.max.y = rect.min.y + height;

			for (int x = 0; x < s_xDivisions; x++)
			{
        rect.min.x = offset.x + x * width;
        rect.max.x = rect.min.x + width;

				m_children[y * s_xDivisions + x] = new Node(this, rect, m_maxItems, m_depth + 1);
			}
		}

		m_divided = true;
		
		int i = 0;
		while (i < m_items.size())
		{
			//try to put it in a child
			if (!PushItemDown(i))
			{
				//if it failed, go to the next item
				i++;
			}
		}
	}

	void Node::RemoveDivide(void)
	{
		if (!m_divided) { return; }

		//TODO:
		//get items from children
		//add it to this list
		for (auto& child : m_children)
		{
			//get items and add to this node
			child->Destroy();
			delete child;
		}

		m_divided = false;
	}

	bool Node::InsertInChildren(Item *_item)
	{
		//can't add the item to the children if they don't exist.
		if (!m_divided) { return false; }

		Rect itemRect = _item->GetRect();
		bool inserted = false;
		
		for (auto& node : m_children)
		{
			if (node->m_rect.Intersects(itemRect))
			{
				node->Insert(_item);

				inserted = true;

				//if the node fully contains the item, it can't be in 
				//any of the other nodes, so exit the loop.
				if (node->m_rect.Contains(itemRect))
				{
					break;
				}

				//don't quit the loop if the item is partially outside of 
				//the node as the item could overlap and be in multiple nodes.
			}
		}
		
		return inserted;
	}

	bool Node::PushItemDown(int _index)
	{
		if (InsertInChildren(m_items[_index]))
		{
			RemoveItem(_index);
			return true;
		}

		return false;
	}

	bool Node::PushItemUp(int _index)
	{
		if (m_parent == nullptr) { return false; }

		Item* item = m_items[_index];

		RemoveItem(_index);
		m_parent->Insert(item);
		return true;
	}

	void Node::RemoveItem(int _index)
	{
		m_items.erase(m_items.begin() + _index);
	}
}