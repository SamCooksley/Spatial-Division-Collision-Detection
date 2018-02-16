#include "CM_QuadTree.h"

CM_QuadTree::CM_QuadTree() :
  m_quadTree(Rect(0, 0, 800, 600))
{ }

CM_QuadTree::~CM_QuadTree()
{ }

void CM_QuadTree::Reset()
{
  m_quadTree.Reset();
}

void CM_QuadTree::Collide()
{
  QuadTree::PairList<Collider> pairs;
  m_quadTree.GetPairs(pairs);

  for (auto& pair : pairs)
  {
    CollisionManager::Collide(*pair.a, *pair.b);
  }
}

void CM_QuadTree::Draw(Renderer& _renderer)
{
  m_quadTree.Draw(_renderer);
}

void CM_QuadTree::Insert(const std::shared_ptr<Collider>& _collider)
{
  m_quadTree.Insert(_collider.get());
}