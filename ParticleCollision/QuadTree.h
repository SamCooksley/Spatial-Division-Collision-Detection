#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include "QuadTree_Node.h"

namespace QuadTree
{
  /**
   * \brief Define a quad tree.
   * Contain the quad tree and define 
   * some variable to use of it's 
   * generation.
   */
  template <class T>
  class QuadTree
  {
    friend class Node<T>;

   public:

    /**
     * \brief Constructor.
     * \param [in] _rect Area to cover.
     */
    QuadTree(const Rect& _rect) :
      m_maxItems(10), m_maxDepth(5),
      m_xDivisions(2), m_yDivisions(2)
    {
      Resize(_rect);
    }

    ~QuadTree()
    { } //!< Destructor.

    /**
     * \brief Set the area for the tree to cover.
     * \param [in] _rect Position and size of the root.
     */
    void Resize(const Rect& _rect)
    {
      m_root.reset(
        new Node<T>(this, nullptr, _rect, 0)
      );
    }

    void Reset()
    {
      m_root->Clear();
    } //!< Clear the tree.

    void Insert(T* _item)
    {
      m_root->Insert(_item);
    } //!< Add an item to the tree.

    void Draw(Renderer& _renderer)
    {
      m_root->Draw(_renderer);
    } //!< Draw the nodes.

    /**
     * \brief Perform an operation of each item in a bucket.
     * \param [in] _op Operation to perform each combination of two items in a bucket. Format: void(T*, T*).
     */
    template<typename U>
    void CheckCollision(U& _op)
    {
      m_root->CheckCollision(_op);
    }

   private:
    std::unique_ptr<Node<T>> m_root; //!< Top node of the tree.

    size_t m_maxItems; //!< Maximum items in a bucket, if a bucket passes this value, the node is subdivided.
    int m_maxDepth; //!< Maximum level of nodes.

    int m_xDivisions; //!< Amount of subdivisions on the x axis. 
    int m_yDivisions; //!< Amount of subdivisions on the y axis.
  };
}

#endif //_QUADTREE_H_