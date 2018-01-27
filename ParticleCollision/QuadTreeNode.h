#ifndef _QUADTREENODE_H_
#define _QUADTREENODE_H_

#include <vector>

#include "Rect.h"
#include "QuadTreeItem.h"
#include "Collider.h"

namespace QuadTree
{
  class Node
  {
		friend class Item;
    typedef std::vector<Item*> ItemList;

  private:
    Rect m_rect;
		int m_depth;

    Node *m_parent;

    bool m_divided;

    const static int s_xDivisions = 2;
    const static int s_yDivisions = 2;
    Node *m_children[s_xDivisions * s_yDivisions]; //TODO: maybe vector??

    int m_maxItems;
		ItemList m_items;

  public:
    Node(Node *_parent, const Rect &_rect, int _maxItems, int _depth);
    ~Node(void);

		void Destroy(void);

		void Clear(void);

    void SetMaxItems(int _max);

		void Insert(Item *_item);
		//void Remove(Item &_item);

		int GetItemCount(void) const;

		void Draw(Renderer &_renderer);

		template<typename T>
		void CheckCollision(T _check)
		{
			if (m_divided)
			{
				for (auto& child : m_children)
				{
					child->CheckCollision(_check);
				}
			}

			for (size_t i = 0; i < m_items.size() - 1; ++i)
			{
				Collider &a = *(Collider*)m_items[i]->Get();
				for (size_t j = i + 1; j < m_items.size(); ++j)
				{
					Collider &b = *(Collider*)m_items[j]->Get();
					_check(a, b);
				}
			}
		}

	private:
		void Divide(void);
		void RemoveDivide(void);

		bool InsertInChildren(Item *_item);

		bool PushItemDown(int _index); //moves item to child
		bool PushItemUp(int _index); //moves item to parent

		void RemoveItem(int _index);
  };
}

#endif //_QUADTREENODE_H_