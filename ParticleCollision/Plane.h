#ifndef _PLANE_H_
#define _PLANE_H_

#include "Maths.h"
#include "Collider.h"

/**
 * \brief Define a plane collider.
 */

class Plane : public Collider
{
  friend class CollisionManager;

public:
  Plane(const Vector2& _position, const Vector2& _normal, float _width); //!< Constructor.

  void Draw(Renderer& _renderer) override; //!< Draw the Plane.

  bool CheckCollision(Collider& _other, CollisionData& _data) override; //!< Used for double dispatch.
  
  /**
   * \brief Check if a circle has collided with the plane.
   * \param [in]  _other Circle to check.
   * \param [out] _data  Information about the collision.
   * \return Returns true if to objects overlap.
   */
  bool CheckCollision(Circle& _other, CollisionData& _data) override;
  
  /**
   * \brief Check if a polygon has collided with the plane.
   * \param [in]  _other  Polygon to check.
   * \param [out] _data   Information about the collision.
   * \return Returns true if to objects overlap.
   */
  bool CheckCollision(Polygon& _other, CollisionData& _data) override;
  
  /**
   * \brief Check if there is a collision between the planes.
   * \param [in]  _other Plane to check.
   * \param [out] _data  Information about the collision.
   * \return Returns true if to objects overlap.
   */
  bool CheckCollision(Plane& _other, CollisionData& _data) override;

  /**
   * \brief Get the area of the collider on an axis.
   * \param [in] _axis Axis to project onto.
   * \return Returns the range on an axis.
   */
  Range MinMaxOnAxis(const Vector2& _axis) const override;

  Vector2 Min() const; //!< Get the minimum point in world space.
  Vector2 Max() const; //!< Get the maximum point in world space.

private:
  Vector2 m_normal; //!< Direction of the plane.
  float m_width; //!< Size of the plane.
};

#endif //_PLANE_H_