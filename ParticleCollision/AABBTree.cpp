#include "AABBTree.h"

#include <iostream>

namespace AABBTree
{
	AABBTree::AABBTree(void) :
		m_root(nullptr)
	{
	}

	AABBTree::~AABBTree(void)
	{
	}

	void AABBTree::Insert(Item *_item)
	{
		if (m_root == nullptr)
		{
			m_root = new Node(nullptr);
			m_root->SetLeaf(_item);
		}
		else
		{
			Node *node = new Node(nullptr);
			node->SetLeaf(_item);
			InsertNode(node, &m_root);
		}
	}

	void AABBTree::Remove(Item *_item)
	{

	}

	void AABBTree::Reset(void)
	{
		if (m_root != nullptr)
		{
			delete m_root;
			m_root = nullptr;
		}
	}

	void AABBTree::Update(void)
	{
		if (m_root != nullptr)
		{
			if (m_root->IsLeaf())
			{
				m_root->UpdateAABB();
			}
			else
			{
				m_invalidNodes.clear();

				GetInvalidNodes(m_root, m_invalidNodes);

				for (Node *node : m_invalidNodes)
				{
					Node *parent = node->m_parent;
					Node *sibling = node->GetSibling();
					Node **parentLink = nullptr;
					if (parent->m_parent != nullptr)
					{
						parentLink = &parent->m_parent->m_children[parent->GetChildIndex()];
					}
					else
					{
						parentLink = &m_root;
					}

					sibling->m_parent = (parent->m_parent != nullptr)
						? parent->m_parent
						: nullptr;

					*parentLink = sibling;
					delete parent;

					node->UpdateAABB();
					InsertNode(node, &m_root);
				}

				m_invalidNodes.clear();
			}
		}
	}

	ColliderPairList &AABBTree::GetColliderPairs(void)
	{
		//resest the list
		m_pairs.clear();

		//if there is one or less colliders in the list
		//there can't be any pairs
		if (m_root == nullptr ||
				m_root->IsLeaf())
		{
			return m_pairs;
		}

		ResetCross(m_root);
		CalculatePairs(m_root->m_children[0], m_root->m_children[1]);

		return m_pairs;
	}

	void AABBTree::Draw(Renderer &_renderer)
	{
		_renderer.SetRenderColour(255, 0, 0);
		if (m_root != nullptr)
		{
			m_root->Draw(_renderer);
		}
	}

	void AABBTree::InsertNode(Node *_node, Node **_parent)
	{
		Node *p = *_parent;
		if (p->IsLeaf())
		{
			Node *newParent = new Node(p->m_parent);
			newParent->SetBranch(_node, p);
			*_parent = newParent;
		}
		else
		{
			const Rect &child1 = p->m_children[0]->m_rect;
			const Rect &child2 = p->m_children[1]->m_rect;

			const Rect &node = _node->m_rect;

			const float volume1 = Rect::Union(child1, node).Area() - child1.Area() - node.Area();
			const float volume2 = Rect::Union(child2, node).Area() - child2.Area() - node.Area();

			if (volume1 < volume2)
			{
				InsertNode(_node, &p->m_children[0]);
			}
			else
			{
				InsertNode(_node, &p->m_children[1]);
			}
		}

		//update AABB
		(*_parent)->UpdateAABB();
	}

	void AABBTree::CalculatePairs(Node *_a, Node *_b)
	{
		if (!_a->m_rect.Intersects(_b->m_rect))
		{
			//return;
		}

		//if (_a == nullptr || _b == nullptr) { return; }

		if (!_a->IsLeaf() &&
			   _b->IsLeaf())
		{
			Node *tmp = _a;
			_a = _b;
			_b = tmp;
		}

		if (_a->IsLeaf())
		{
			if (_b->IsLeaf())
			{
				//if they overlap
				if (_a->m_item->GetRect().Intersects(_b->m_item->GetRect()))
				{
					m_pairs.emplace_back(*(Collider*)_a->m_item->Get(), 
															 *(Collider*)_b->m_item->Get());
				}
			}
			else
			{
				CrossChildren(_b);
				CalculatePairs(_a, _b->m_children[0]);
				CalculatePairs(_a, _b->m_children[1]);
			}
		}
		else
		{
			CrossChildren(_a);
			CrossChildren(_b);

			if (_a->m_rect.Intersects(_b->m_rect))
			{
				CalculatePairs(_a->m_children[0], _b->m_children[0]);
				CalculatePairs(_a->m_children[0], _b->m_children[1]);
				CalculatePairs(_a->m_children[1], _b->m_children[0]);
				CalculatePairs(_a->m_children[1], _b->m_children[1]);
			}
		}
	}

	void AABBTree::CrossChildren(Node *_node)
	{
		if (!_node->m_crossed)
		{
			CalculatePairs(_node->m_children[0], _node->m_children[1]);
			_node->m_crossed = true;
		}
	}

	void AABBTree::ResetCross(Node *_node)
	{
		_node->m_crossed = false;
		if (!_node->IsLeaf())
		{
			ResetCross(_node->m_children[0]);
			ResetCross(_node->m_children[1]);
		}
	}

	void AABBTree::GetInvalidNodes(Node *_node, NodeList &_invalid)
	{
		if (_node->IsLeaf())
		{
			if (!_node->m_rect.Contains(_node->m_item->GetRect()))
			{
				_invalid.emplace_back(_node);
			}
		}
		else
		{
			GetInvalidNodes(_node->m_children[0], _invalid);
			GetInvalidNodes(_node->m_children[1], _invalid);
		}
	}
}