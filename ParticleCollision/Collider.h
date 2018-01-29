#ifndef _COLLIDER_H_
#define _COLLIDER_H_

#include "Maths.h"
#include "Range.h"

#include "Renderer.h"

#include "CollisionData.h"

#include "QuadTree_IItem.h"
#include "AABBTreeItemCollider.h"

class Circle;
class Polygon;
class Plane;

enum class ColliderType
{
  AABB,
  CIRCLE,
  POLYGON,
  PLANE
}; //!< Types of colliders.

/**
 * \brief Base class for colliders.
 */

class Collider : public QuadTree::IItem
{
  friend class CollisionManager;

 public:
  Collider(ColliderType _type, const Vector2& _position, const Vector2& _velocity); //!< Constructor.
  virtual ~Collider(); //!< Destructor.

  virtual void Update(float _deltaTime); //!< Simulate the object. Move the object.
  virtual void Draw(Renderer& _renderer); //!< Render the collider's shape.

  virtual bool CheckCollision(Collider& _other, CollisionData& _data) = 0; //!< Used for double dispatch.
  virtual bool CheckCollision(Circle&   _other, CollisionData& _data) = 0; //!< Collide against cicles.
  virtual bool CheckCollision(Polygon&  _other, CollisionData& _data) = 0; //!< Collide against polygons.
  virtual bool CheckCollision(Plane&    _other, CollisionData& _data) = 0; //!< Collide against planes.

  /**
   * \brief Get the range of the collider on the axis. 
   * \param [in] _axis Axis to project the collider.
   * \return Returns the minimum and maximum on an axis.
   */
  virtual Range MinMaxOnAxis(const Vector2& _axis) const = 0;

  const Vector2& GetPosition() const; //!< Get the position.
  const Vector2& GetVelocity() const; //!< Get the velocity. 

  const Rect& GetAABB() const; //!< Get the bounding box.

  void DrawRect(Renderer& _renderer) const;

	AABBTree::Item* AsAABBItem();

 protected:
  Vector2 m_position; //!< position.
  Vector2 m_velocity; //!< velocity.

  float m_invMass; //!< Inverse mass.
  float m_bounciness; //!< energy loss.

  Rect m_aabb; //!< Bounds of the collider.

  AABBTree::ItemCollider m_aabbItem; //!< Object used for aabb trees.

 private:
  ColliderType m_type; //!< Type of collider.
};

#endif //_COLLIDER_H_