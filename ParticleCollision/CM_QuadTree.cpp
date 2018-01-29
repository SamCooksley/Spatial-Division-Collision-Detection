#include "CM_QuadTree.h"

CM_QuadTree::CM_QuadTree() :
  m_quadTree(Vector2(800, 600))
{ }

CM_QuadTree::~CM_QuadTree()
{ }

void CM_QuadTree::Reset()
{
  m_quadTree.Reset();
}

void CM_QuadTree::Collide()
{
  //define what to do with objects in a bucket.
  //this should not be needs as Collide is a static method.
  auto collide = [this](Collider* _a, Collider* _b)
  {
    CollisionManager::Collide(*_a, *_b);
  };

  m_quadTree.CheckCollision(collide);
}

void CM_QuadTree::Draw(Renderer& _renderer)
{
  m_quadTree.Draw(_renderer);
}

void CM_QuadTree::Insert(const std::shared_ptr<Collider>& _collider)
{
  m_quadTree.Insert(_collider.get());
}