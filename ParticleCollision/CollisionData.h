#ifndef _COLLISIONDATA_H_
#define _COLLISIONDATA_H_

#include "Maths.h"

class Collider;

/**
 * \brief Store information about a collision.
 */

struct CollisionData
{
 public:
  float overlap; //!< Collision depth.
  Vector2 normal; //!< Normal of the collision from the persective of a.
  Vector2 position; //!< collision point.
  Collider *a; //!< First collider.
  Collider *b; //!< Second collider.
};

#endif //_COLLISIONDATA_H_