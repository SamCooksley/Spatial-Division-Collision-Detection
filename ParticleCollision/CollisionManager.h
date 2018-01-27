#ifndef _COLLISIONMANAGER_H_
#define _COLLISIONMANAGER_H_

#include <vector>
#include <memory>

#include "Collider.h"
#include "Polygon.h"
#include "Circle.h"
#include "Plane.h"

#include "CollisionData.h"
#include "QuadTree.h"
#include "AABBTree.h"

/**
 * \brief Stores functionality for and managees collisions.
 */

class CollisionManager
{
 public:
	enum class BroadPhaseType
	{
		NONE,
		QUAD,
		AABB
	}; //!< Types of broad-phase

  CollisionManager(); //!< Constructor.

  void Collide(); //!< Check for collisions and resolve them.
	void Draw(Renderer& _renderer); //!< Draw the broad-base tree.

  void Add(const std::shared_ptr<Collider>& _collider); //!< Add a collider to be affected.

  QuadTree::QuadTree& GetQuadTree(); //!< Get the quad tree.
  AABBTree::AABBTree& GetAABBTree(); //!< Get the AABB tree.

  void SetBroadPhase(BroadPhaseType _type); //!< Set the type of broad-phase to use.

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
  static bool CollisionOnAxis(const Collider& _a, const Collider& _b, const Vector2& _axis);
  
  /**
   * \brief Check if two circles are intersecting.
   * \param [in]  _a
   * \param [in]  _b
   * \param [out] _data Information about the collision.
   * \return Returns true if the circles overlap.
   */
  static bool CheckCollision(const Circle& _a, const Circle& _b, CollisionData& _data);
  
  /**
   * \brief Check if a circle is intersecting a polygon.
   * \param [in]  _a
   * \param [in]  _b
   * \param [out] _data Information about the collision.
   * \return Returns true objects are colliding.
   */
  static bool CheckCollision(const Circle& _a, const Polygon& _b, CollisionData& _data);
  
  /**
   * \brief Check if a circle is intersecting a plane.
   * \param [in]  _a
   * \param [in]  _b
   * \param [out] _data Information about the collision.
   * \return Returns true objects are colliding.
   */
  static bool CheckCollision(const Circle& _a, const Plane& _b, CollisionData& _data);
 
  /**
   * \brief Check if two polygons are intersecting.
   * \param [in]  _a
   * \param [in]  _b
   * \param [out] _data Information about the collision.
   * \return Returns true objects are colliding.
   */
  static bool CheckCollision(const Polygon& _a, const Polygon& _b, CollisionData& _data);
  
  /**
   * \brief Check if a polygon is intersecting with a plane.
   * \param [in]  _a
   * \param [in]  _b
   * \param [out] _data Information about the collision.
   * \return Returns true objects are colliding.
   */
  static bool CheckCollision(const Polygon& _a, const Plane& _b, CollisionData& _data);

  /**
   * \brief Check all the edge normals of polygon a with b.
   * \param [in]  _a    Polygon with edge checks.
   * \param [in]  _b    Other polygon.
   * \param [out] _data Information about the collision.
   * \return Returns true objects are colliding.
   */
  static bool CheckEdgeCollisions(const Polygon& _a, const Polygon& _b, CollisionData& _data);

  /**
   * \brief Resolve a collion between to colliders.
   * \param [in, out] _a
   * \param [in, out] _b
   * \param [in]      _data Information about the collision.
   */
  static void ResolveCollision(Collider& _a, Collider& _b, const CollisionData& _data);

 private:
  BroadPhaseType m_broadPhase; //!< Type of broad-phase being used.

  std::vector<std::shared_ptr<Collider>> m_colliders; //!< List of colliders to use.

  QuadTree::QuadTree m_quadTree; //!< Quad tree broad-phase.
  AABBTree::AABBTree m_aabbTree; //!< AABB tree broad-phase.
};

#endif //_COLLISIONMANAGER_H_