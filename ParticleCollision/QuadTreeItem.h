#ifndef _QUADTREEITEM_H_
#define _QUADTREEITEM_H_

#include "Rect.h"
#include <vector>

namespace QuadTree
{
	class Node;

  class Item
  {
		friend class Node;
	private:
		std::vector<Node*> m_nodes;

  public:
    Item(void);
    virtual ~Item(void);

    virtual Vector2 GetPosition(void) const = 0;
    virtual Rect GetRect(void) const = 0;

		virtual void *Get(void) = 0;

		void Draw(Renderer &_renderer);
  };
}

#endif //_QUADTREEITEM_H_