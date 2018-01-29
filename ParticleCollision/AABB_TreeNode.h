#ifndef _AABBTREENODE_H_
#define _AABBTREENODE_H_

#include <array>
#include <vector>
#include <exception>

#include "Rect.h"

#include "AABBTree_IItem.h"

namespace AABBTree
{
  template <class T>
  class AABBTree;

  template <class T>
  class Node;

  template <class T>
  using NodeList = std::vector<Node<T>*>;

  template <class T>
  class Node
  {
    friend class AABBTree<T>;    

   public:
    /**
     * \brief Constructor.
     * \param [in] _tree   Tree that the node belongs to.
     * \param [in] _parent Parent node.
     */
    Node(AABBTree<T>* _tree, Node<T>* _parent) :
      m_tree(_tree), m_parent(_parent),
      m_item(nullptr), m_crossed(false)
    {
      m_children[0] = m_children[1] = nullptr;
    }

    ~Node()
    { } //!< Destructor.

    /**
     * \brief Set the children of the node.
     * \param [in] _a First child.
     * \param [in] _b Second child.
     */
    void SetBranch(Node<T>* _a, Node<T>* _b)
    {
      _a->m_parent = this;
      _b->m_parent = this;

      m_children[0] = _a;
      m_children[1] = _b;
    }

    /**
     * \brief Set the node to contain an item.
     * Deletes children if they exist.
     * \param [in] _item Item to store.
     */
    void SetLeaf(T* _item)
    {
      DestroyChildren();

      m_item = _item;

      UpdateAABB();
    }

    bool IsLeaf() const
    {
      return m_children[0] == nullptr;
    } //!< Does the node contain an item?

    Node<T>* GetSibling()
    {
      return m_parent->m_children[!GetChildIndex()];
    } //!< Get the other node of the parent.

    int GetChildIndex() const
    {
      if (m_parent == nullptr)
      {
        throw std::runtime_error("Parent is null.");
      }
      return (this == m_parent->m_children[0] ? 0 : 1);
    } //!< Get the child number for the parent.

    /**
     * \brief Update the bounding box to contain nodes/items.
     */
    void UpdateAABB()
    {
      //if it has an item, update the bounds to the item's bounds (+ padding).
      if (IsLeaf())
      {
        const Rect& itemRect = m_item->GetAABB();
        Vector2 padding(m_tree->m_padding);
        m_rect = Rect(itemRect.min - padding, itemRect.max + padding);
      }
      //else contain the children.
      else
      {
        m_rect = Rect::Union(m_children[0]->m_rect, m_children[1]->m_rect);
      }
    }

    void GetInvalidNodes(NodeList<T>& _invalid)
    {
      //check if the item is fully contained by the node, else add it to
      //the invalid list.
      if (IsLeaf())
      {
        if (!m_rect.Contains(m_item->GetAABB()))
        {
          _invalid.emplace_back(this);
        }
      }
      //if it children, recursively check them.
      else
      {
        m_children[0]->GetInvalidNodes(_invalid);
        m_children[1]->GetInvalidNodes(_invalid);
      }
    }

    void Draw(Renderer& _renderer)
    {
      m_rect.Draw(_renderer);

      if (IsLeaf())
      {
        m_item->DrawRect(_renderer);
      }
      else
      {
        m_children[0]->Draw(_renderer);
        m_children[1]->Draw(_renderer);
      }
    } //!< Draw the bounding box and children.

   private:
    void DestroyChildren()
    {
      delete m_children[0];
      delete m_children[1];
      m_children[0] = m_children[1] = nullptr;
    } //!< Delete children.

    AABBTree<T>* m_tree; //!< Tree the node belongs to.
    Node<T>* m_parent; //!< Parent node.

    std::array<Node<T>*, 2> m_children; //!< Nodes inside this node.
    T* m_item; //!< Item contained by the node.

    Rect m_rect; //!< Size of the node.

    bool m_crossed; //!< Has the node been searched?
  };
}

#endif //_AABBTREENODE_H_