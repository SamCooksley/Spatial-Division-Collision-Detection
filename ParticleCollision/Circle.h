#ifndef _CIRCLE_H_
#define _CIRCLE_H_

#include "Collider.h"

/**
 * \brief Define a collider with a circle shape.
 */

class Circle : public Collider
{
  friend class CollisionManager;

 public:
  Circle(const Vector2 &_position, const Vector2 &_velocity, float _radius); //!< Constructor.

  void Draw(Renderer &_renderer) override; //!< Draw the circle.

  bool CheckCollision(const Collider &_other, CollisionData& _data) const override; //!< Used for double dispatch.
  
  /**
   * \brief Check if two circles have collided.
   * \param [in]  _other Circle to check.
   * \param [out] _data  Information about the collision.
   * \return Returns true if to objects overlap.
   */
  bool CheckCollision(const Circle& _other, CollisionData& _data) const override;
  
  /**
   * \brief Check if another the plane has collided with a polygon.
   * \param [in]  _other  Polygon to check.
   * \param [out] _data   Information about the collision.
   * \return Returns true if to objects overlap.
   */
  bool CheckCollision(const Polygon& _other, CollisionData& _data) const override;
  
  /**
   * \brief Check if there is a collision between a plane and the circle.
   * \param [in]  _other Plane to check.
   * \param [out] _data  Information about the collision.
   * \return Returns true if to objects overlap.
   */
  bool CheckCollision(const Plane& _other, CollisionData& _data) const override;

  /**
   * \brief Get the area of the collider on an axis.
   * \param [in] _axis Axis to project onto.
   * \return Returns the range on an axis.
   */
  Range MinMaxOnAxis(const Vector2 &_axis) const override;

 private:
  float m_radius; //!< Size of the circle.
};

#endif //_CIRCLE_H_