#ifndef _CM_QUADTREE_H_
#define _CM_QUADTREE_H_

#include "CollisionManager.h"

#include "QuadTree.h"

class CM_QuadTree : public CollisionManager
{
 public:
  CM_QuadTree();
  ~CM_QuadTree();

  void Reset() override;

  void Collide() override;
  void Draw(Renderer& _renderer) override;

  void Insert(const std::shared_ptr<Collider>& _collider) override;

 private:
   QuadTree::QuadTree<Collider> m_quadTree;
};

#endif //_CM_QUADTREE_H_