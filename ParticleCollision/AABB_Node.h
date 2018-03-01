#ifndef _AABBTREENODE_H_
#define _AABBTREENODE_H_

#include <array>
#include <vector>
#include <exception>

#include "Rect.h"

#include "AABBTree_IItem.h"
#include "AABB_Pair.h"

namespace AABBTree
{
  template <class T>
  class AABBTree;

  template <class T>
  class Node;

  template <class T>
  using NodeList = std::vector<Node<T>*>;

  using NodeIndex = int;

  /**
   * \brief Element in an aabb tree.
   * Leaf nodes contain an item. Other nodes contain two other child nodes.
   */
  template <class T>
  class Node
  {
    friend class AABBTree<T>;    

   public:
     static const NodeIndex Null = -1;
    /**
     * \brief Constructor.
     * \param [in] _tree   Tree that the node belongs to.
     * \param [in] _parent Parent node.
     */
    Node() :
      parent(Null),
      item(nullptr), crossed(false)
    {
      children[0] = children[1] = Null;
    }

    ~Node()
    { } //!< Destructor.

    bool IsLeaf() const
    {
      return children[0] == Null;
    } //!< Does the node contain an item?

    union
    {
      NodeIndex parent;
      NodeIndex next;
    };

    std::array<NodeIndex, 2> children; //!< Nodes inside this node.
    T* item; //!< Item contained by the node.

    Rect rect; //!< Size of the node.

    bool crossed; //!< Has the node been searched?
  };
}

#endif //_AABBTREENODE_H_