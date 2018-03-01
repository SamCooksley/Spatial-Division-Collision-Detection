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

    using NodeType = Node<T>;
    using NodeList = std::vector<NodeType>;

   public:
    AABBTree(size_t _poolSize = 16) : 
      m_root(NodeType::Null),
      m_freeList(NodeType::Null), 
      m_padding(2.f)
    { 
      AllocateNodes(_poolSize);
    } //!< Constructor.

    ~AABBTree()
    { } //!< Destructor.

    /**
     * \brief Insert an item into the tree.
     * \param [in] _item Item to insert.
     */
    void Insert(T* _item)
    {
      NodeIndex node = CreateNode();

      const Rect& rect = _item->GetAABB();
      Vector2 pad(m_padding, m_padding);
      m_nodes[node].rect.min = rect.min - pad;
      m_nodes[node].rect.max = rect.max + pad;
      m_nodes[node].item = _item;

      InsertNode(node);
    }

    /** 
     * \brief Remove an item from the tree.
     * \param [in] _item Item to remove.
     */
    void Remove(T* _item)
    {
      NodeIndex node = Find(_item);
      //Find the node that contains the item.
      if (node != NodeType::Null)
      {
        FreeNode(node);
      }
    }

    void Reset()
    {
      FreeNode(m_root);
    } //!< clear the tree.

    /**
     * \brief Update the structure of the tree.
     * Check if the nodes still contain their items.
     */
    void Update()
    {
      //if there is no tree, exit.
      if (m_root == NodeType::Null) { return; }


      std::vector<NodeIndex> invalid;

      //get all the nodes where their items have exited their bounds.
      GetInvalidNodes(m_root, invalid);

      for (size_t i = 0; i < invalid.size(); ++i)
      {
        Update(invalid[i]);
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
      if (m_root == NodeType::Null ||
          m_nodes[m_root].IsLeaf())
      {
        return;
      }

      //get the pairs.
      ResetCross(m_root);
      CalculatePairs(m_nodes[m_root].children[0], m_nodes[m_root].children[1], _pairs);
    }

    void Draw(Renderer& _renderer)
    {
      if (m_root == NodeType::Null) { return; }
      Draw(_renderer, m_root);
    } //!< Draw the tree nodes.

  private:
    /**
     * \brief Insert a node.
     * \param [in] _node Node to insert.
     */
    void InsertNode(NodeIndex _node)
    {
      if (m_root == NodeType::Null)
      {
        m_root = _node;
        m_nodes[m_root].parent = NodeType::Null;
        return;
      }

      Rect insertRect = m_nodes[_node].rect;
      NodeIndex index = m_root;

      while (m_nodes[index].IsLeaf() == false)
      {
        float area = m_nodes[index].rect.Area();

        float combinedArea = Rect::Union(m_nodes[index].rect, insertRect).Area();

        float cost = 2.f * combinedArea;
        float inheritanceCost = 2.f * (combinedArea - area);
        
        bool set = false;
        NodeIndex cur = index;

        for (int i = 0; i < m_nodes[cur].children.size(); ++i)
        {
          float descendCost = .0f;
          NodeIndex child = m_nodes[cur].children[i];
          if (m_nodes[child].IsLeaf())
          {
            descendCost = Rect::Union(m_nodes[child].rect, insertRect).Area() + inheritanceCost;
          }
          else
          {
            float currentArea = m_nodes[child].rect.Area();
            float newArea = Rect::Union(m_nodes[child].rect, insertRect).Area();
            descendCost = newArea - currentArea + inheritanceCost;
          }
          
          if (descendCost < cost || i == 0)
          {
            cost = descendCost;
            index = child;
            set = true;
          }
        }

        if (!set) { break; }
      }

      NodeIndex sibling = index;

      NodeIndex oldParent = m_nodes[sibling].parent;
      NodeIndex newParent = CreateNode(oldParent);
      m_nodes[newParent].rect = Rect::Union(m_nodes[sibling].rect, insertRect);

      if (oldParent != NodeType::Null)
      {
        m_nodes[oldParent].children[GetChildIndex(sibling)] = newParent;
      }
      else
      {
        m_root = newParent;
      }

      m_nodes[newParent].children[0] = sibling;
      m_nodes[newParent].children[1] = _node;

      m_nodes[sibling].parent = newParent;
      m_nodes[_node].parent = newParent;

      index = newParent;
      while (index != NodeType::Null)
      {
        NodeIndex child1 = m_nodes[index].children[0];
        NodeIndex child2 = m_nodes[index].children[1];

        m_nodes[index].rect = Rect::Union(m_nodes[child1].rect, m_nodes[child2].rect);

        index = m_nodes[index].parent;
      }
    }

    /**
     * \brief Remove a node from the tree.
     * Restuctures the tree to accomodate the change.
     * \param [in] _node Node to remove.
     */
    void RemoveNode(NodeIndex _node)
    {
      if (_node == m_root)
      {
        m_root = NodeType::Null;
        return;
      }

      //if there is no parent, there is no sibling to manage.
      //if (m_nodes[_node].parent == NodeType::Null) { return; }

      //replace the sibling as the parent.

      NodeIndex parent = m_nodes[_node].parent;
      NodeIndex grandParent = m_nodes[parent].parent;
      NodeIndex sibling = m_nodes[parent].children[!GetChildIndex(_node)];

      if (grandParent != NodeType::Null)
      {
        m_nodes[sibling].parent = grandParent;
        m_nodes[grandParent].children[GetChildIndex(parent)] = sibling;

        FreeNode(parent);
      }
      else
      {
        m_root = sibling;
        m_nodes[sibling].parent = NodeType::Null;
        FreeNode(parent);
      }
    }

    void Update(NodeIndex _node)
    {
      if (m_nodes[_node].IsLeaf() == false)
      {
        return;
      }

      const Rect& rect = m_nodes[_node].item->GetAABB();

      if (m_nodes[_node].rect.Contains(rect))
      {
        return;
      }

      RemoveNode(_node);

      Vector2 pad(m_padding, m_padding);
      m_nodes[_node].rect.min = rect.min - pad;
      m_nodes[_node].rect.max = rect.max + pad;

      InsertNode(_node);
    }

    void GetInvalidNodes(NodeIndex _node, std::vector<NodeIndex>& _outInvalid)
    {
      if (m_nodes[_node].IsLeaf())
      {
        if (m_nodes[_node].rect.Contains(m_nodes[_node].item->GetAABB()) == false)
        {
          _outInvalid.push_back(_node);
        }
      }
      else
      {
        GetInvalidNodes(m_nodes[_node].children[0], _outInvalid);
        GetInvalidNodes(m_nodes[_node].children[1], _outInvalid);
      }
    }

    /**
     * \brief Get the item pairs that overlap.
     * \param [in]  _a
     * \param [in]  _b     
     * \param [out] _pairs List of overlapping items.
     */
    void CalculatePairs(NodeIndex _a, NodeIndex _b, PairList<T>& _outPairs)
    {
      //if one is a leaf, make sure it is a.
      if (m_nodes[_a].IsLeaf() == false &&
          m_nodes[_b].IsLeaf() == true)
      {
        NodeIndex tmp = _a;
        _a = _b;
        _b = tmp;
      }

      //if a has an item.
      if (m_nodes[_a].IsLeaf())
      {
        //if b also has an item
        if (m_nodes[_b].IsLeaf())
        {
          //check if items can collide, add them to the list.
          T* aItem = m_nodes[_a].item;
          T* bItem = m_nodes[_b].item;

          if (Rect::Intersects(aItem->GetAABB(), bItem->GetAABB()))
          {
            Pair<T> pair = { aItem, bItem };
            _outPairs.emplace_back(pair);
          }
        }
        //if b has children, check a with the children.
        else
        {
          ChildPairs(_b, _outPairs);
          CalculatePairs(_a, m_nodes[_b].children[0], _outPairs);
          CalculatePairs(_a, m_nodes[_b].children[1], _outPairs);
        }
      }
      else
      {
        //find pairs inside children. 
        ChildPairs(_a, _outPairs);
        ChildPairs(_b, _outPairs);

        //if the node overlap, check all the children between them.0
        if (Rect::Intersects(m_nodes[_a].rect, m_nodes[_b].rect))
        {
          NodeIndex a1 = m_nodes[_a].children[0];
          NodeIndex a2 = m_nodes[_a].children[1];
          NodeIndex b1 = m_nodes[_b].children[0];
          NodeIndex b2 = m_nodes[_b].children[1];

          CalculatePairs(a1, b1, _outPairs);
          CalculatePairs(a1, b2, _outPairs);
          CalculatePairs(a2, b1, _outPairs);
          CalculatePairs(a2, b2, _outPairs);
        }
      }
    }

    void ChildPairs(NodeIndex _node, PairList<T>& _outPairs)
    {
      if (m_nodes[_node].crossed == false)
      {
        CalculatePairs(m_nodes[_node].children[0], m_nodes[_node].children[1], _outPairs);
        m_nodes[_node].crossed = true;
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

    NodeIndex CreateNode(NodeIndex _parent = NodeType::Null)
    {
      if (m_freeList == NodeType::Null)
      {
        AllocateNodes(m_nodes.size());
      }

      NodeIndex node = m_freeList;

      m_freeList = m_nodes[node].next;

      m_nodes[node].parent = _parent;
      return node;
    }

    void AllocateNodes(size_t _num)
    {
      if (_num == 0) { return; }

      NodeIndex start = m_nodes.size();

      m_nodes.resize(start + _num);

      for (size_t i = start; i < m_nodes.size() - 1; ++i)
      {
        m_nodes[i].next = i + 1;
      }
      m_nodes.back().next = m_freeList;
      m_freeList = start;
    }

    void FreeNode(NodeIndex _node)
    {
      if (_node == NodeType::Null) { return; }

      if (!m_nodes[_node].IsLeaf())
      {
        //TODO: free children
      }

      m_nodes[_node].next = m_freeList;
      m_freeList = _node;
    }

    NodeIndex Find(NodeIndex _node, const T* _item)
    {
      if (m_nodes[_node].IsLeaf())
      {
        return m_nodes[_node].item == _item ? _node : NodeType::Null;
      }
      else
      {
        NodeIndex node = Find(m_nodes[_node].children[0], _item);
        if (node == NodeType::Null)
        {
          node = Find(m_nodes[_node].children[1], _item);
        }
        return node;
      }
    }

    size_t GetChildIndex(NodeIndex _node)
    {
      NodeIndex parent = m_nodes[_node].parent;
      if (parent == NodeType::Null)
      {
        throw std::out_of_range("Node has no parent");
      }

      return m_nodes[parent].children[0] == _node ? 0u : 1u;
    }

    void Draw(Renderer& _renderer, NodeIndex _node)
    {
      m_nodes[_node].rect.Draw(_renderer);

      if (m_nodes[_node].IsLeaf())
      {
        m_nodes[_node].item->GetAABB().Draw(_renderer);
      }
      else
      {
        Draw(_renderer, m_nodes[_node].children[0]);
        Draw(_renderer, m_nodes[_node].children[1]);
      }
    }

    void ResetCross(NodeIndex _node)
    {
      m_nodes[_node].crossed = false;
      if (m_nodes[_node].IsLeaf() == false)
      {
        ResetCross(m_nodes[_node].children[0]);
        ResetCross(m_nodes[_node].children[1]);
      }
    }

    NodeList m_nodes; 

    NodeIndex m_root;
    NodeIndex m_freeList;

    float m_padding; //!< Extra padding on items for leaf nodes.
  };
}

#endif //_AABBTREE_H_