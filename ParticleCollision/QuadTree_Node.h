#ifndef _QUADTREENODE_H_
#define _QUADTREENODE_H_

#include <vector>
#include <memory>

#include "QuadTree_IItem.h"

namespace QuadTree
{
  template <class T>
  class QuadTree;

  template <class T>
  class Node
  {
    typedef std::vector<T*> ItemList;

  public:
    Node(QuadTree<T>* _tree, Node<T>* _parent, const Rect& _rect, int _depth) :
      m_tree(_tree), m_parent(_parent), 
      m_rect(_rect), m_depth(_depth)
    { }

    ~Node()
    { }

    void Clear()
    {
      m_items.clear();
      RemoveDivide();
    }

    void Insert(T* _item)
    {
      if (!InsertInChildren(_item))
      {
        m_items.emplace_back(_item);

        if (m_depth < m_tree->m_maxDepth &&
            m_items.size() >= m_tree->m_maxItems)
        {
          Divide();
        }
      }
    }
    //void Remove(Item &_item);

    size_t GetItemCount() const
    {
      size_t count = m_items.size();

      //add up all the children's item counts.
      for (auto& child : m_children)
      {
        count += child->GetItemCount();
      }

      return count;
    }

    bool IsLeaf() const
    {
      return m_children.empty();
    }

    void Draw(Renderer& _renderer)
    {
      m_rect.Draw(_renderer);

      //draw the children
      for (auto& child : m_children)
      {
        child->Draw(_renderer);
      }
      
      //draw the items in the node
      for (auto& item : m_items)
      {
        item->DrawRect(_renderer);
      }
    }

    template<typename U>
    void CheckCollision(U& _check)
    {
      for (auto& child : m_children)
      {
        child->CheckCollision(_check);
      }

      if (m_items.empty()) { return; }

      for (size_t i = 0; i < m_items.size() - 1; ++i)
      {
        T* a = m_items[i];
        for (size_t j = i + 1; j < m_items.size(); ++j)
        {
          T* b = m_items[j];
          _check(a, b);
        }
      }
    }

	 private:
    void Divide()
    {
      if (!IsLeaf()) { return; }

      int xCount = m_tree->m_xDivisions;
      int yCount = m_tree->m_yDivisions;

      float width = m_rect.Width() / xCount;
      float height = m_rect.Height() / yCount;

      Rect rect;

      Vector2 offset = m_rect.min;

      m_children.resize(xCount * yCount);

      for (int y = 0; y < yCount; ++y)
      {
        rect.min.y = offset.y + y * height;
        rect.max.y = rect.min.y + height;

        for (int x = 0; x < xCount; ++x)
        {
          rect.min.x = offset.x + x * width;
          rect.max.x = rect.min.x + width;

          m_children[y * xCount + x].reset(
            new Node(m_tree, this, rect, m_depth + 1)
          );
        }
      }

      size_t i = 0; 
      while (i < m_items.size())
      {
        if (!PushItemDown(i))
        {
          ++i;
        }
      }
    }

    void RemoveDivide()
    {
      m_children.clear();
    }

    /**
     * \brief Add an item to the children.
     * \param [in] _item Item to insert.
     * \return Returns true if the object was inserted into at least one child. 
     */
    bool InsertInChildren(T* _item)
    {
      if (IsLeaf()) { return false; }

      const Rect& itemRect = _item->GetAABB();
      bool inserted = false;

      for (auto& node : m_children)
      {
        if (Rect::Intersects(node->m_rect, itemRect))
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

    /**
     * \brief Move an item to a child.
     * \param [in] _index Item to move.
     * \return Returns true if the object was move to a child.
     */
    bool PushItemDown(int _index)
    {
      if (InsertInChildren(m_items[_index]))
      {
        RemoveItem(_index);
        return true;
      }

      return false;
    }

    /**
     * \brief Move an item to the parent.
     * \param [in] _index Item index to move up.
     */
		bool PushItemUp(int _index)
    {
      if (m_parent == nullptr) { return false; }

      T* item = m_items[_index];

      RemoveItem(_index);
      m_parent->m_items.emplace_back(item);
      //m_parent->Insert(item);
      return true;
    }

    void RemoveItem(int _index)
    {
      m_items.erase(m_items.begin() + _index);
    } //!< Clear an item from the bucket.

    QuadTree<T>* m_tree; 
    Node<T>* m_parent;

    Rect m_rect;
    int m_depth;

    std::vector<std::unique_ptr<Node>> m_children;
    ItemList m_items;
  };
}

#endif //_QUADTREENODE_H_