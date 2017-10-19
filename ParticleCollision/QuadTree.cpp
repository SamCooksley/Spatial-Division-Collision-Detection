#include "QuadTree.h"

namespace QuadTree
{
  QuadTree::QuadTree(const Rect &_world, int _maxItemsPerNode) :
    m_root(nullptr, _world, _maxItemsPerNode, 0)
  {
   
  }

  QuadTree::QuadTree(const Vector2 &_worldSize, int _maxItemsPerNode) :
    QuadTree(Rect(Vector2(0, 0), _worldSize), _maxItemsPerNode)
  { }

  QuadTree::~QuadTree(void)
  {
  }

	void QuadTree::Insert(Item *_item)
	{
		//TODO: if it does not fit, resize root

		m_root.Insert(_item);
	}

	void QuadTree::Draw(Renderer &_renderer)
	{
		_renderer.SetRenderColour(255, 0, 0);
		m_root.Draw(_renderer);
	}
}