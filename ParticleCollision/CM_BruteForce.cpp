#include "CM_BruteForce.h"

CM_BruteForce::CM_BruteForce()
{ }

CM_BruteForce::~CM_BruteForce()
{ }

void CM_BruteForce::Reset()
{
  m_colliders.clear();
}

void CM_BruteForce::Collide()
{
  if (m_colliders.empty()) { return; }

  //go through all combonations and test for a collision.
  for (size_t i = 0; i < m_colliders.size() - 1; ++i)
  {
    for (size_t j = i + 1; j < m_colliders.size(); ++j)
    {
      CollisionManager::Collide(*m_colliders[i], *m_colliders[j]);
    }
  }
}

void CM_BruteForce::Draw(Renderer& _renderer)
{
  for (auto& collider : m_colliders)
  {
    collider->GetAABB().Draw(_renderer);
  }
}

void CM_BruteForce::Insert(const std::shared_ptr<Collider>& _collider)
{
  m_colliders.emplace_back(_collider.get());
}