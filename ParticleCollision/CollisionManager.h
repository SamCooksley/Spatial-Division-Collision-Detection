#ifndef _COLLISIONMANAGER_H_
#define _COLLISIONMANAGER_H_

#include <vector>
#include <memory>

#include "Collider.h"
#include "Polygon.h"
#include "Circle.h"
#include "Plane.h"

#include "CollisionData.h"

enum class BroadPhaseType
{
  NONE,
  QUAD,
  AABB
}; //!< Types of broad-phase

/**
 * \brief Stores functionality for and manages collisions.
 */

class CollisionManager
{
 public:
  CollisionManager(); //!< Constructor.
  virtual ~CollisionManager();

  virtual void Reset() = 0;

  virtual void Collide() = 0;
  virtual void Draw(Renderer& _renderer) = 0;

  virtual void Insert(const std::shared_ptr<Collider>& _collider) = 0;

  /**
   * \brief Do collision check and response.
   * \param [in, out] _lhs
   * \param [in, out] _rhs
   */
  static void Collide(Collider& _lhs, Collider& _rhs);

  /**
   * \brief Check if colliders overlap on an axis.
   * \param [in] _a
   * \param [in] _b
   * \param [in] _axis Axis to check.
   * \return Returns true if the objects overlap.
   */
  static bool CollisionOnAxis(Collider& _a, Collider& _b, const Vector2& _axis);
  
  /**
   * \brief Check if two circles are intersecting.
   * \param [in]  _a
   * \param [in]  _b
   * \param [out] _data Information about the collision.
   * \return Returns true if the circles overlap.
   */
  static bool CheckCollision(Circle& _a, Circle& _b, CollisionData& _data);
  
  /**
   * \brief Check if a circle is intersecting a polygon.
   * \param [in]  _a
   * \param [in]  _b
   * \param [out] _data Information about the collision.
   * \return Returns true objects are colliding.
   */
  static bool CheckCollision(Circle& _a, Polygon& _b, CollisionData& _data);
  
  /**
   * \brief Check if a circle is intersecting a plane.
   * \param [in]  _a
   * \param [in]  _b
   * \param [out] _data Information about the collision.
   * \return Returns true objects are colliding.
   */
  static bool CheckCollision(Circle& _a, Plane& _b, CollisionData& _data);
 
  /**
   * \brief Check if two polygons are intersecting.
   * \param [in]  _a
   * \param [in]  _b
   * \param [out] _data Information about the collision.
   * \return Returns true objects are colliding.
   */
  static bool CheckCollision(Polygon& _a, Polygon& _b, CollisionData& _data);
  
  /**
   * \brief Check if a polygon is intersecting with a plane.
   * \param [in]  _a
   * \param [in]  _b
   * \param [out] _data Information about the collision.
   * \return Returns true objects are colliding.
   */
  static bool CheckCollision(Polygon& _a, Plane& _b, CollisionData& _data);

  /**
   * \brief Check all the edge normals of polygon a with b.
   * \param [in]  _a    Polygon with edge checks.
   * \param [in]  _b    Other polygon.
   * \param [out] _data Information about the collision.
   * \return Returns true objects are colliding.
   */
  static bool CheckEdgeCollisions(Polygon& _a, Polygon& _b, CollisionData& _data);

  /**
   * \brief Resolve a collion between to colliders.
   * \param [in, out] _a
   * \param [in, out] _b
   * \param [in]      _data Information about the collision.
   */
  static void ResolveCollision(Collider& _a, Collider& _b, const CollisionData& _data);
};

#endif //_COLLISIONMANAGER_H_