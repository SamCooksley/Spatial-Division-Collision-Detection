#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include "QuadTree_Node.h"
#include "QuadTree_Pair.h"

#include <iostream>

namespace QuadTree
{
  /**
   * \brief Define a quad tree.
   * Contain the quad tree and define 
   * some variable to use of it's 
   * generation.
   */
  template <class T, size_t DivX, size_t DivY>
  class QuadTree
  {
    friend class Node<T, DivX * DivY>;

    using NodeType = Node<T, DivX * DivY>;
    using NodeList = std::vector<NodeType>;

   public:
    /**
     * \brief Constructor.
     * \param [in] _rect     Area to cover.
     * \param [in] _poolSize Initial node pool size.
     */
    QuadTree(const Rect& _rect, size_t _poolSize = 16) :
      m_maxItems(10), m_maxDepth(5),
      m_root(NodeType::Null), m_freeList(NodeType::Null)
    {
      AllocateNodes(_poolSize);

      m_root = CreateNode(NodeType::Null, _rect, 0);
    }

    ~QuadTree()
    { } //!< Destructor.

    /**
     * \brief Set the area for the tree to cover.
     * \param [in] _rect Position and size of the root.
     */
    void Reset(const Rect& _rect)
    {
      FreeNode(m_root);
      m_root = CreateNode(NodeType::Null, _rect, 0);
    }

    void Reset()
    {
      Reset(m_nodes[m_root].rect);
    } //!< Clear the tree.

    void Insert(T* _item)
    {
      InsertInto(m_root, _item);
    } //!< Add an item to the tree.

    void Draw(Renderer& _renderer)
    {
      Draw(m_root, _renderer);
    } //!< Draw the nodes.

    /**
     * \brief Get a list of potential overlaps.
     * \param [out] _pairs List of items that potentially overlap.
     */
    void GetPairs(PairList<T>& _pairs)
    {
      GetPairs(m_root, _pairs);
    }

   private:
     /**
      * \brief Insert an item into a node.
      * If the node is a leaf, add the item to the node.
      * Else insert into children.
      * \param [in] _node Node to insert into.
      * \param [in] _item Item to add. 
      */
     void InsertInto(NodeIndex _node, T* _item)
     {
       //if the item is not within the nodes rect, don't add it.
       if (!Rect::Intersects(m_nodes[_node].rect, _item->GetAABB()))
       {
         return;
       }

       //if it is a leaf, add it to the bucket.
       if (m_nodes[_node].IsLeaf())
       {
         m_nodes[_node].items.emplace_back(_item);

         //if the bucket has too many items, divide the node.
         //if it has reached the max depth, don't divide.
         if (m_nodes[_node].depth < m_maxDepth &&
           m_nodes[_node].items.size() >= m_maxItems)
         {
           DivideNode(_node);
         }
       }
       //if the node has children, add the item to it's children.
       else
       {
         for (size_t i = 0; i < m_nodes[_node].children.size(); ++i)
         {
           InsertInto(m_nodes[_node].children[i], _item);
         }
       }
     }

     /**
      * \brief Get the pairs of items that could potentially overlap. 
      * \param [in]  _node  Node to get pairs from.
      * \param [out] _pairs List of items that potentially overlap.
      */
     void GetPairs(NodeIndex _node, PairList<T>& _pairs)
     {
       //if it is a leaf, add items to the pair list.
       if (m_nodes[_node].IsLeaf())
       {
         //if there are no items, there are no pairs, so exit.
         //needed to prevent use of -1 on size_t (unsigned int)
         if (m_nodes[_node].items.empty()) { return; }

         //compare all combinations of items.
         for (size_t i = 0; i < m_nodes[_node].items.size() - 1; ++i)
         {
           T* a = m_nodes[_node].items[i];
           for (size_t j = i + 1; j < m_nodes[_node].items.size(); ++j)
           {
             T* b = m_nodes[_node].items[j];
             //if the two items overlap, add them to the pair list.
             if (Rect::Intersects(a->GetAABB(), b->GetAABB()))
             {
               _pairs.push_back({ a, b });
             }
           }
         }
       }
       //if is not a leaf, get the pairs of the children. 
       else
       {
         for (int i = 0; i < m_nodes[_node].children.size(); ++i)
         {
           GetPairs(m_nodes[_node].children[i], _pairs);
         }
       }
     }

     /**
      * \brief Create child node for a node.
      * \param [in] _node Node to divide.
      */
     void DivideNode(NodeIndex _node)
     {
       //if it has already been divied, exit.
       if (!m_nodes[_node].IsLeaf()) { return; }

       //get the size of child node rects.
       float width = m_nodes[_node].rect.Width()   / static_cast<float>(DivX);
       float height = m_nodes[_node].rect.Height() / static_cast<float>(DivY);

       Rect rect;
       Vector2 offset = m_nodes[_node].rect.min;

       //y divisions.
       for (size_t y = 0; y < DivY; ++y)
       {
         rect.min.y = offset.y + y * height;
         rect.max.y = rect.min.y + height;

         //x divisions.
         for (size_t x = 0; x < DivX; ++x)
         {
           rect.min.x = offset.x + x * width;
           rect.max.x = rect.min.x + width;

           //create the child
           m_nodes[_node].children[y * DivX + x] = CreateNode(_node, rect, m_nodes[_node].depth + 1);
         }
       }

       //add all the items to the children.
       for (size_t i = 0; i < m_nodes[_node].items.size(); ++i)
       {
         InsertInto(_node, m_nodes[_node].items[i]);
       }
       //clear items.
       m_nodes[_node].items.clear();
     }

     /**
      * \brief Remove children of a node.
      * \param [in] _node Node to modify.
      */
     void RemoveDivide(NodeIndex _node)
     {
       //if the node has no children, exit.
       if (m_nodes[_node].IsLeaf()) { return; }

       for (size_t i = 0; i < m_nodes[_node].children.size(); ++i)
       {
         NodeIndex child = m_nodes[_node].children[i];
         //make the child a leaf.
         RemoveDivide(child);

         //add the child items to the node.
         m_nodes[_node].items.insert(m_nodes[_node].items.end(),
           m_nodes[child].items.begin(),
           m_nodes[child].items.end()
         );

         //remove the child node.
         FreeNode(child);
         m_nodes[_node].children[i] = NodeType::Null;
       }
     }

     /**
      * \brief Draw the node's area.
      * If the node is a leaf, draw its children.
      * Else draw the child nodes.
      * \param [in]      _node     Node to draw.
      * \param [in, out] _renderer Renderer to draw to.
      */
     void Draw(NodeIndex _node, Renderer& _renderer) const
     {
       m_nodes[_node].rect.Draw(_renderer);

       //if it is a leaf, draw the items.
       if (m_nodes[_node].IsLeaf())
       {
         for (size_t i = 0; i < m_nodes[_node].items.size(); ++i)
         {
           m_nodes[_node].items[i]->DrawRect(_renderer);
         }
       }
       //if it is not leaf, draw the children.
       else
       {
         for (size_t i = 0; i < m_nodes[_node].children.size(); ++i)
         {
           Draw(m_nodes[_node].children[i], _renderer);
         }
       }
     }

     /**
      * \brief Get a free node.
      * \param [in] _parent
      * \param [in] _rect  
      * \param [in] _depth 
      * \return Returns the new node.
      */
     NodeIndex CreateNode(NodeIndex _parent, const Rect& _rect, int _depth)
     {
       //if the are no free nodes, create more.
       if (m_freeList == NodeType::Null)
       {
         //double the allocated nodes.
         AllocateNodes(m_nodes.size());
       }

       //get a free node.
       NodeIndex nodeIndex = m_freeList;
       //update the free list.
       m_freeList = m_nodes[nodeIndex].next;

       //setup the new mode.
       m_nodes[nodeIndex].parent = _parent;
       m_nodes[nodeIndex].rect = _rect;
       m_nodes[nodeIndex].depth = _depth;

       return nodeIndex;
     }

     /**
      * \brief Create new nodes to add the object pool.
      * \param [in] _num Number of nodes to create. 
      */
     void AllocateNodes(size_t _num)
     {
       NodeIndex start = m_nodes.size();
     
       //add the new nodes.
       m_nodes.resize(start + _num);
       
       //link the new nodes.
       for (size_t i = start; i < m_nodes.size() - 1; ++i)
       {
         m_nodes[i].next = i + 1;
       }
       //insert the new nodes to the free list.
       m_nodes[m_nodes.size() - 1].next = m_freeList;
       m_freeList = start;
     }

     /**
      * \brief Destroy a node.
      * Add a node to the free list 
      * \param [in] _node Node to free.
      */
     void FreeNode(NodeIndex _node)
     {
       //if it is not a node, exit.
       if (_node == NodeType::Null) { return; }

       //free the node's children.
       if (!m_nodes[_node].IsLeaf())
       {
         for (size_t i = 0; i < m_nodes[_node].children.size(); ++i)
         {
           FreeNode(m_nodes[_node].children[i]);
           m_nodes[_node].children[i] = -1;
         }
       }

       m_nodes[_node].Reset();

       //insert the node to the free list.
       m_nodes[_node].next = m_freeList;
       m_freeList = _node;
     }


    NodeIndex m_root; //!< Top node of the tree.

    NodeIndex m_freeList; //!< First node that is not used.

    NodeList m_nodes; //!< All the nodes that the tree uses and the pooled nodes.

    size_t m_maxItems; //!< Maximum items in a bucket, if a bucket passes this value, the node is subdivided.
    int m_maxDepth; //!< Maximum level of nodes.
   };
}

#endif //_QUADTREE_H_