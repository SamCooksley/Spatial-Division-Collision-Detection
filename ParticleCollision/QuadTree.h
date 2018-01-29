#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include "QuadTree_Node.h"

namespace QuadTree
{
  //TODO: write a resize function.
  template <class T>
  class QuadTree
  {
    friend class Node<T>;

   public:
    QuadTree(const Rect& _world) :
      m_maxItems(10), m_maxDepth(5),
      m_xDivisions(2), m_yDivisions(2)
    {
      m_root.reset(
        new Node<T>(this, nullptr, _world, 0)
      );
    }
    QuadTree(const Vector2& _worldSize) :
      QuadTree(Rect(Vector2(.0f, .0f), _worldSize))
    { }
    ~QuadTree()
    { }

		void Reset()
		{
			m_root->Clear();
		}

    void Insert(T* _item)
    {
      m_root->Insert(_item);
    }

    void Draw(Renderer& _renderer)
    {
      m_root->Draw(_renderer);
    }

		template<typename U>
		void CheckCollision(U& _check)
		{
			m_root->CheckCollision(_check);
		}

   private:
    std::unique_ptr<Node<T>> m_root;

    size_t m_maxItems;
    int m_maxDepth;

    int m_xDivisions;
    int m_yDivisions;
  };
}

#endif //_QUADTREE_H_