#ifndef _AABBTREE_H_
#define _AABBTREE_H_

#include "AABB_Node.h"

#include "AABB_Pair.h"

#include <iostream>

namespace AABBTree
{
  /**
   * \brief Define an aabb tree.
   * Store tree and define functions for 
   * the tree to operate.
   */
  template <class T>
  class AABBTree
  {
    friend class Node<T>;

   public:
    AABBTree() : 
      m_root(nullptr), m_padding(2.f)
    { } //!< Constructor.

    ~AABBTree()
    { } //!< Destructor.

    /**
     * \brief Insert an item into the tree.
     * \param [in] _item Item to insert.
     */
    void Insert(T* _item)
    {
      //if there is no root, create a root and add the item to it.
      if (m_root == nullptr)
      {
        m_root = new Node<T>(this, nullptr);
        m_root->SetLeaf(_item);
      }
      //Create a node for item and insert the node into the tree.
      else
      {
        Node<T>* node = new Node<T>(this, nullptr);
        node->SetLeaf(_item);
        InsertNode(node, m_root);
      }
    }

    /** 
     * \brief Remove an item from the tree.
     * \param [in] _item Item to remove.
     */
    void Remove(T* _item)
    {
      Node<T>* node = nullptr;
      //Find the node that contains the item.
      if (m_root->Find(_item, node))
      {
        RemoveNode(node);
        delete node;
      }
    }

    void Reset()
    {
      delete m_root;
      m_root = nullptr;
    } //!< clear the tree.

    /**
     * \brief Update the structure of the tree.
     * Check if the nodes still contain their items.
     */
    void Update()
    {
      //if there is no tree, exit.
      if (m_root == nullptr) { return; }

      //if there is only the root, update it.
      if (m_root->IsLeaf())
      {
        m_root->UpdateAABB();
      }
      //else find all the nodes that need updating.
      else
      {
        NodeList<T> invalid;

        //get all the nodes where their items have exited their bounds.
        m_root->GetInvalidNodes(invalid);

        for (auto& node : invalid)
        {
          RemoveNode(node);

          node->UpdateAABB();
          //reinsert the node to the tree.
          auto r = m_root;
          InsertNode(node, m_root);
          m_root = r;
        }
      }
    }

    /**
     * \brief Get a list of item pairs.
     * \param [out] _pairs Pairs in the tree.
     */
    void GetColliderPairs(PairList<T>& _pairs)
    {
      _pairs.clear();

      //if there is no tree, other there is only
      //one item, exit.
      if (m_root == nullptr ||
          m_root->IsLeaf())
      {
        return;
      }

      //get the pairs.
      m_root->ResetCross();
      CalculatePairs(m_root->m_children[0], m_root->m_children[1], _pairs);

      return;
    }

    void Draw(Renderer& _renderer)
    {
      if (m_root != nullptr)
      {
        m_root->Draw(_renderer);
      }
    } //!< Draw the tree nodes.

  private:
    /**
     * \brief Insert a node.
     * \param [in]      _node   Node to insert.
     * \param [in, out] _parent Parent of the new node. Can be newly created node.
     */
    void InsertNode(Node<T>* _node, Node<T>*& _parent)
    {
      Node<T>* p = _parent;
      //if the node has an item, split it and create a new parent.
      if (p->IsLeaf())
      {
        Node<T>* newParent = new Node<T>(this, p->m_parent);
        newParent->SetBranch(_node, p);
        _parent = newParent;
      }
      //else add it to the child with the smallest rect.
      else
      {
        const Rect& childRect1 = p->m_children[0]->m_rect;
        const Rect& childRect2 = p->m_children[1]->m_rect;

        const Rect& nodeRect = _node->m_rect;

        float area1 = Rect::Union(childRect1, nodeRect).Area();
        float area2 = Rect::Union(childRect2, nodeRect).Area();

        //insert the node to the node that has a smaller area.
        if (area1 < area2)
        {
          InsertNode(_node, p->m_children[0]);
        }
        else
        {
          InsertNode(_node, p->m_children[1]);
        }
      }

      //update the parent's aabb as it has new area.
      _parent->UpdateAABB();
    }

    /**
     * \brief Remove a node from the tree.
     * Restuctures the tree to accomodate the change.
     * \param [in] _node Node to remove.
     */
    void RemoveNode(Node<T>* _node)
    {
      //if there is no parent, there is no sibling to manage.
      if (_node->m_parent == nullptr) { return; }

      //replace the sibling as the parent.

      Node<T>* p = _node->m_parent;
      Node<T>* sib = _node->GetSibling();

      //link the sibling to the parent's parent.
      if (p->m_parent != nullptr)
      {
        sib->m_parent = p->m_parent;
        sib->m_parent->m_children[p->GetChildIndex()] = sib;
        sib->m_parent->UpdateAABB();
      }
      else
      {
        sib->m_parent = nullptr;
      }

      //clear the parent from the removed node.
      _node->m_parent = nullptr;

      //get rid of the old parent that is not used.
      delete p;
    }

    /**
     * \brief Get the item pairs that overlap.
     * \param [in]  _a
     * \param [in]  _b     
     * \param [out] _pairs List of overlapping items.
     */
    void CalculatePairs(Node<T>* _a, Node<T>* _b, PairList<T>& _pairs)
    {
      //if one is a leaf, make sure it is a.
      if (!_a->IsLeaf() && _b->IsLeaf())
      {
        Node<T>* tmp = _a;
        _a = _b;
        _b = tmp;
      }

      //if a has an item.
      if (_a->IsLeaf())
      {
        //if b also has an item
        if (_b->IsLeaf())
        {
          //check if items can collide, add them to the list.
          if (Rect::Intersects(_a->m_item->GetAABB(), _b->m_item->GetAABB()))
          {
            Pair<T> pair = { _a->m_item, _b->m_item };
            _pairs.emplace_back(pair);
          }
        }
        //if b has children, check a with the children.
        else
        {
          ChildPairs(_b, _pairs);
          CalculatePairs(_a, _b->m_children[0], _pairs);
          CalculatePairs(_a, _b->m_children[1], _pairs);
        }
      }
      else
      {
        //find pairs inside children. 
        ChildPairs(_a, _pairs);
        ChildPairs(_b, _pairs);

        //if the node overlap, check all the children between them.0
        if (Rect::Intersects(_a->m_rect, _b->m_rect))
        {
          CalculatePairs(_a->m_children[0], _b->m_children[0], _pairs);
          CalculatePairs(_a->m_children[0], _b->m_children[1], _pairs);
          CalculatePairs(_a->m_children[1], _b->m_children[0], _pairs);
          CalculatePairs(_a->m_children[1], _b->m_children[1], _pairs);
        }
      }
    }

    /**
     * \brief Get item pairs for the children.
     * \param [in, out] _node  Parent node.
     * \param [out]     _pairs List of items that overlap.
     */
    void ChildPairs(Node<T>* _node, PairList<T>& _pairs)
    {
      if (!_node->m_crossed)
      {
        CalculatePairs(_node->m_children[0], _node->m_children[1], _pairs);
        _node->m_crossed = true;
      }
    }

    Node<T>* m_root; //!< Top of the tree.
    float m_padding; //!< Extra padding on items for leaf nodes.
  };
}

#endif //_AABBTREE_H_