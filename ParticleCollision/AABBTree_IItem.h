#ifndef _AABBTREEITEM_H_
#define _AABBTREEITEM_H_

#include "Rect.h"

namespace AABBTree
{
  /**
   * \brief Interface for items inside an aabb tree.
   */
  class IItem
  {
   public:
    virtual const Vector2& GetPosition() const = 0;
    virtual const Rect& GetAABB() const = 0;

    virtual void DrawRect(Renderer& _renderer) const = 0;
  };
}

#endif //_AABBTREEITEM_H_