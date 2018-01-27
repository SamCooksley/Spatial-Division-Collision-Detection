#ifndef _POLYGON_H_
#define _POLYGON_H_

#include "Collider.h"

/**
 * \brief Define a polygon collider.
 */

class Polygon : public Collider
{
  friend class CollisionManager;

 public:
  Polygon(const Vector2& _position, const Vector2& _velocity); //!< Constructor.

  /**
   * \brief Create a random polygon.
   * \param [in] _minSize     Minimum distance from the origin.
   * \param [in] _maxSize     Maximum distance from the origin.
   * \param [in] _vertexCount Number of vertices to create.
   */
  void GenerateRandom(float _minSize, float _maxSize, int _vertexCount);

  void Draw(Renderer& _renderer) override; //!< Draw the polygon.

  bool CheckCollision(const Collider& _other, CollisionData& _data) const override; //!< Used for double dispatch.
  
  /**
   * \brief Check if a circle has collided with the polygon.
   * \param [in]  _other Circle to check.
   * \param [out] _data  Information about the collision.
   * \return Returns true if to objects overlap.
   */
  bool CheckCollision(const Circle& _other, CollisionData& _data) const override; 
  
  /**
   * \brief Check if another polygon has collided with the polygon.
   * \param [in]  _other  Polygon to check.
   * \param [out] _data   Information about the collision.
   * \return Returns true if to objects overlap.
   */
  bool CheckCollision(const Polygon& _other, CollisionData& _data) const override;
  
  /**
   * \brief Check if there is a collision between a plane and the polygon.
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
  Range MinMaxOnAxis(const Vector2& _axis) const override;

 private:
  std::vector<Vector2> m_points; //!< List of points for the corners (vertices) of the polygon.
};

#endif //_POLYGON_H_