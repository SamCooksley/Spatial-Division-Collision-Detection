#ifndef _RECT_H_
#define _RECT_H_

#include "Maths.h"
#include "Renderer.h"

/**
 * \brief Define a rectangle.
 * AABB.
 */

class Rect
{
 public:
   Vector2 min; //!< minimum corner of the rectangle (top left).
   Vector2 max; //!< maximum corner or the rectang;e (bottom right).

	Rect(); //!< default Constructor.
	/**
	 * \brief Constructor.
	 * \param [in] _minimum Define the top left corner.
	 * \param [in] _maximum Define the bottom left corner.
	 */
  Rect(const Vector2& _min, const Vector2& _max);
	/**
	 * \brief Constructor.
	 * \param [in] _minx Left.
	 * \param [in] _miny Top.
	 * \param [in] _maxx Right.
	 * \param [in] _maxy Bottom.
	 */
  Rect(float _minx, float _miny, float _maxx, float _maxy);

	float Width()  const; //!< Get the width.
	float Height() const; //!< Get the height.
	Vector2 Size() const; //!< Get the width and height.

	float Parimeter() const; //!< Get the parimeter.
	float Area()      const; //!< Get the area.

  Vector2 TopLeft()     const; //!< minimum x, minimum y.
  Vector2 TopRight()    const; //!< maximum x, minimum y.
  Vector2 BottomLeft()  const; //!< minimim x, maximum y.
  Vector2 BottomRight() const; //!< maximum x, maximum y.

	/**
	 * \brief Checks if the point is within the rectangle.
	 * \param [in] _point The point to query.
	 * \return Returns true if the the point is inside the rectangle.
	 */
  bool Contains(const Vector2& _point) const;

	/**
	 * \brief Checks if the rectangle is fully contained.
	 * \param [in] _rect Rect to query.
	 * \return Returns true if the rectangle is contained within this Rectangle.
	 */
	bool Contains(const Rect& _rect)     const;

	/**
	 * \brief Checks if the rectangle overlaps this.
	 * \param [in] _rect Rect to query.
	 * \return Returns true if the rectangles overlap.
	 */
  bool Intersects(const Rect& _rect)   const;

	/** 
	 * \brief Draws the rectangle.
	 * \param [in] _renderer Renderer to draw to.
	 */
	void Draw(Renderer& _renderer);

	/**
	 * \brief Checks if the rectangles overlap.
	 * \param [in] _a
   * \param [in] _b
	 * \return Returns true if the rectangles overlap.
	 */
	static bool Intersects(const Rect& _a, const Rect& _b);

	/**
	 * \brief Creates a rectangle that encompases both.
	 * \param [in] _a
	 * \param [in] _b
	 * \return Returns the new rectangle. 
	 */
	static Rect Union(const Rect& _a, const Rect& _b);
};

#endif //_RECT_H_