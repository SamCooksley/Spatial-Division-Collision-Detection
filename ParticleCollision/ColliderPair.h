#ifndef _COLLIDERPAIR_H_
#define _COLLIDERPAIR_H_

#include <vector>

#include "Collider.h"

struct ColliderPair
{
 public: 
	Collider *a;
	Collider *b;
};

typedef std::vector<ColliderPair> ColliderPairList;

#endif //_COLLIDERPAIR_H_