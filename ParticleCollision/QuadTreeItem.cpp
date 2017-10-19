#include "QuadTreeItem.h"

#include "QuadTreeNode.h"

namespace QuadTree
{
	Item::Item(void)
	{
	}

  Item::~Item(void)
  {
  }

	void Item::Draw(Renderer &_renderer)
	{
		GetRect().Draw(_renderer);
	}
}