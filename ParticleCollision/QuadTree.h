#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include "QuadTreeNode.h"

namespace QuadTree
{
  //TODO: write a resize function.
  class QuadTree
  {
  private:
    Node m_root;

  public:
    QuadTree(const Rect &_world, int _maxItemsPerNode);
    QuadTree(const Vector2 &_worldSize, int _maxItemsPerNode);
    ~QuadTree(void);

		void Reset(void)
		{
			m_root.Destroy();
		}

		void Clear(void)
		{
			m_root.Clear();
		}

    void Insert(Item *_item);

		void Draw(Renderer &_renderer);

		template<typename T>
		void CheckCollision(T _check)
		{
			m_root.CheckCollision(_check);
		}
  };
}

#endif //_QUADTREE_H_