#ifndef _COLLIDERPAIR_H_
#define _COLLIDERPAIR_H_

#include <vector>

#include "Collider.h"

class ColliderPair
{
private: 
	Collider *m_a;
	Collider *m_b;

public:
	ColliderPair(Collider &_a, Collider &_b);
	~ColliderPair(void);

	Collider &A(void);
	Collider &B(void);
};

typedef std::vector<ColliderPair> ColliderPairList;

#endif //_COLLIDERPAIR_H_