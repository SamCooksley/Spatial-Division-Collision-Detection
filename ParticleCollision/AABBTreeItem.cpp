#include "AABBTreeItem.h"

namespace AABBTree
{
	Item::Item()
	{
	}


	Item::~Item()
	{
	}

	void Item::Draw(Renderer &_renderer)
	{
		GetRect().Draw(_renderer);
	}
}
