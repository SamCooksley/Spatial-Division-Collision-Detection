#ifndef _AABBTREE_H_
#define _AABBTREE_H_

#include "AABBTreeNode.h"

#include "ColliderPair.h"

namespace AABBTree
{
	class AABBTree
	{
	private:
		typedef std::vector<Node*> NodeList;

		Node *m_root;

		ColliderPairList m_pairs;
		NodeList m_invalidNodes;

	public:
		AABBTree(void);
		~AABBTree(void);

		void Insert(Item *_item);
		void Remove(Item *_item);

		//TODO: this stuff
		void Insert(const Rect &_aabb, void *_data);

		void Reset(void);

		void Update(void);

		ColliderPairList &GetColliderPairs(void);

		void Draw(Renderer &_renderer);

	private:
		void InsertNode(Node *_node, Node **_parent);

		void CalculatePairs(Node *_a, Node *_b);
		void CrossChildren(Node *_node);
		void ResetCross(Node *_node);
		void GetInvalidNodes(Node *_node, NodeList &_invalid);
	};
}

#endif //_AABBTREE_H_