#include "CM_AABBTree.h"

CM_AABBTree::CM_AABBTree()
{ }

CM_AABBTree::~CM_AABBTree()
{ }

void CM_AABBTree::Reset()
{ }

void CM_AABBTree::Collide()
{
  m_aabbTree.Update();

  auto& pairs = m_aabbTree.GetColliderPairs();

  for (auto& pair : pairs)
  {
    CollisionManager::Collide(*pair.a, *pair.b);
  }
}

void CM_AABBTree::Draw(Renderer& _renderer)
{
  m_aabbTree.Draw(_renderer);
}

void CM_AABBTree::Insert(const std::shared_ptr<Collider>& _collider)
{ }

void CM_AABBTree::Add(const std::shared_ptr<Collider>& _collider)
{
  m_aabbTree.Insert(_collider->AsAABBItem());
}