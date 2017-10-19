#ifndef _ITEMCOLLIDER_H_
#define _ITEMCOLLIDER_H_

#include "QuadTreeItem.h"

class Collider;

//TODO: how to deal with the collider being deleted?? how to detect it?
namespace  QuadTree
{
  class ItemCollider : public Item
  {
  private:
    Collider *m_collider;

  public:
    ItemCollider(Collider &_collider);
    ~ItemCollider(void);

    Vector2 GetPosition(void) const override;
    Rect GetRect(void) const override;

		void* Get(void) override { return m_collider; }
  };
}

#endif //_ITEMCOLLIDER_H_