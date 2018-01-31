#ifndef _QUADTREENODE_H_
#define _QUADTREENODE_H_

#include <vector>
#include <memory>

#include "QuadTree_IItem.h"

namespace QuadTree
{
  template <class T>
  class QuadTree;

  /**
   * \brief Element in a quad tree.
   * A leaf contains a bucket of items. Other nodes contain child nodes.
   */
  template <class T>
  class Node
  {
   public:
    using ItemList = std::vector<T*>; //!< List of items.
    using ItemPairFunc = void(*)(T*, T*); //!< Operation for a pair of items.

    /**
     * \brief Constructor.
     * \param [in] _tree   Tree the node belongs to.
     * \param [in] _parent Parent node.
     * \param [in] _rect   Area of the node.
     * \param [in] _depth  Level of the node.
     */
    Node(QuadTree<T>* _tree, Node<T>* _parent, const Rect& _rect, int _depth) :
      m_tree(_tree), m_parent(_parent), 
      m_rect(_rect), m_depth(_depth)
    { }

    ~Node()
    { } //!< Destructor.

    void Clear()
    {
      m_items.clear();
      RemoveDivide();
    } //!< Remvoe children and items.

    /**
     * \brief Add an item into the node or its children.
     * \param [in] _item Item to add.
     */
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

    /**
     * \brief Get the number of items in the node and its children.
     */
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
    } //!< Does the node have children?


    /**
     * \brief Draw the node and its children.
     * Draws the area of the node and the bounds of the items
     * contained within it.
     * \param [in, out] _renderer Renderer to draw to.
     */
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

    /**
     * \brief Perform an operation of each combination of items in the bucket.
     * \param [in] _operation Operation to perform for each combination.
     */
    void CheckCollision(ItemPairFunc _operation)
    {
      for (auto& child : m_children)
      {
        child->CheckCollision(_operation);
      }

      if (m_items.empty()) { return; }

      for (size_t i = 0; i < m_items.size() - 1; ++i)
      {
        T* a = m_items[i];
        for (size_t j = i + 1; j < m_items.size(); ++j)
        {
          T* b = m_items[j];
          _operation(a, b);
        }
      }
    }

   private:
    /**
     * \brief Divide the area of the node into multiple child nodes.
     * The amount of divisions is dependant on the tree that owns the
     * node.
     */
    void Divide()
    {
      //if has already been divided, exit.
      if (!IsLeaf()) { return; }

      int xCount = m_tree->m_xDivisions;
      int yCount = m_tree->m_yDivisions;

      //get the size of new nodes.
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

      //add the items to the children.
      size_t i = 0; 
      while (i < m_items.size())
      {
        //add the item to the child nodes, if it
        //failed, skip the item.
        if (!PushItemDown(i))
        {
          ++i;
        }
      }
    }

    void RemoveDivide()
    {
      m_children.clear();
    } //!< Delete all children.

    /**
     * \brief Add an item to the children.
     * \param [in] _item Item to insert.
     * \return Returns true if the object was inserted into at least one child. 
     */
    bool InsertInChildren(T* _item)
    {
      //if it has no children, exit.
      if (IsLeaf()) { return false; }

      const Rect& itemRect = _item->GetAABB();
      bool inserted = false;

      //check if the item is inside any of the nodes.
      for (auto& node : m_children)
      {
        if (Rect::Intersects(node->m_rect, itemRect))
        {
          node->Insert(_item);
          inserted = true;
          

          //TODO: profile this to see if it is an improvement.
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
      //if there is no parent to push to, exit.
      if (m_parent == nullptr) { return false; }

      T* item = m_items[_index];

      m_parent->m_items.emplace_back(item);
      RemoveItem(_index);
      return true;
    }

    void RemoveItem(int _index)
    {
      m_items.erase(m_items.begin() + _index);
    } //!< Clear an item from the bucket.

    QuadTree<T>* m_tree; //!< Tree the node belongs to.
    Node<T>* m_parent; //!< Parent node.

    std::vector<std::unique_ptr<Node>> m_children; //!< Child nodes.

    ItemList m_items; //!< List of items in the bucket.

    Rect m_rect; //!< Area that the node covers.
    int m_depth; //!< Level of the node in the tree.
  };
}

#endif //_QUADTREENODE_H_