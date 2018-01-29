#ifndef _CM_AABBTREE_H_
#define _CM_AABBTREE_H_

#include "CollisionManager.h"
#include "AABBTree.h"

class CM_AABBTree : public CollisionManager
{
 public:
  CM_AABBTree();
  ~CM_AABBTree();

  void Reset() override;

  void Collide() override;
  void Draw(Renderer& _renderer) override;

  void Insert(const std::shared_ptr<Collider>& _collider) override;

  //TODO: 
  void Add(const std::shared_ptr<Collider>& _collider);

 private:
  AABBTree::AABBTree<Collider> m_aabbTree;
};

#endif //_CM_AABBTREE_H_