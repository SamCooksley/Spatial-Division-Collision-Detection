#ifndef _QUADTREEITEM_H_
#define _QUADTREEITEM_H_

#include <vector>

#include "Rect.h"

namespace QuadTree
{
  class IItem
  {
   public:
    virtual const Vector2& GetPosition() const = 0;
    virtual const Rect& GetAABB() const = 0;

    virtual void DrawRect(Renderer& _renderer) const = 0;
  };
}

#endif //_QUADTREEITEM_H_