#ifndef _ITEMCOLLIDER_H_
#define _ITEMCOLLIDER_H_

#include "QuadTreeItem.h"

class Collider;

//TODO: how to deal with the collider being deleted?? how to detect it?
namespace  QuadTree
{
  class ItemCollider : public Item
  {
   public:
    ItemCollider(Collider& _collider);
    ~ItemCollider();

    Vector2 GetPosition() const override;
    Rect GetRect() const override;

		void* Get() override { return m_collider; }

   private:
    Collider* m_collider;
  };
}

#endif //_ITEMCOLLIDER_H_