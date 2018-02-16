#ifndef _QUADTREENODE_H_
#define _QUADTREENODE_H_

#include <vector>
#include <array>

#include "QuadTree_IItem.h"

namespace QuadTree
{
  using NodeIndex = int;

  /**
   * \brief Element in a quad tree.
   * A leaf contains a bucket of items. Other nodes contain child nodes.
   */
  template <class T, size_t ChildCount>
  class Node
  {
   public:
     static const NodeIndex Null = -1;

     Node()
     {
       Reset();
     } //!< Constructor.

    bool IsLeaf() const
    {
      return children[0] == -1;
    } //!< Does the node have children?

    void Reset()
    {
      parent = -1;
      depth = -1;
      items.clear();

      for (size_t i = 0; i < children.size(); ++i)
      {
        children[i] = -1;
      }
    } //!< Reset the nodes data.

    union
    {
      NodeIndex parent; //!< Parent node.
      NodeIndex next; //!< next free node.
    };

    int depth; //!< Level of the node in the tree.

    std::array<NodeIndex, ChildCount> children; //!< Child nodes.
    std::vector<T*> items; //!< List of items in the bucket.

    Rect rect; //!< Area that the node covers.
  };
}

#endif //_QUADTREENODE_H_