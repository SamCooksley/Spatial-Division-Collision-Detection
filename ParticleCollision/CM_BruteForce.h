#ifndef _CM_BRUTEFORCE_H_
#define _CM_BRUTEFORCE_H_

#include "CollisionManager.h"

class CM_BruteForce : public CollisionManager
{
 public:
   CM_BruteForce();
  ~CM_BruteForce();

  void Reset() override;

  void Collide() override;
  void Draw(Renderer& _renderer) override;

  void Insert(const std::shared_ptr<Collider>& _collider) override;

 private:
   std::vector<Collider*> m_colliders;
};

#endif //_CM_BRUTEFORCE_H_