#include "ColliderPair.h"

ColliderPair::ColliderPair(Collider &_a, Collider &_b) :
	m_a(&_a), m_b(&_b)
{
}

ColliderPair::~ColliderPair(void)
{
}

Collider &ColliderPair::A(void)
{
	return *m_a;
}

Collider &ColliderPair::B(void)
{
	return *m_b;
}