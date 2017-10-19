#ifndef _RECT_H_
#define _RECT_H_

#include "Vector2.h"
#include "Renderer.h"

/**
 * \brief Define a rectangle.
 * AABB.
 */

struct Rect
{
private:
  Vector2 m_minimum; //!< minimum corner of the rectangle (top left).
  Vector2 m_maximum; //!< maximum corner or the rectang;e (bottom right).

public:
	Rect(void); //!< Constructor.
	/**
	 * \brief Constructor.
	 * \param [in] _minimum Define the top left corner.
	 * \param [in] _maximum Define the bottom left corner.
	 */
  Rect(const Vector2 &_minimum, const Vector2 &_maximum);
	/**
	 * \brief Constructor.
	 * \param [in] _minX Left.
	 * \param [in] _minY Top.
	 * \param [in] _maxX Right.
	 * \param [in] _maxY Bottom.
	 */
  Rect(float _minX, float _minY, float _maxX, float _maxY);
  ~Rect(void); //!< Destructor.

	/**
	 * \brief Reset the values of the rectangle.
	 * \param [in] _minimum Minimum corner.
	 * \param [in] _maximum Maximum corner.
	 */
  void Set(const Vector2 &_minimum, const Vector2 &_maximum);

	/**
	 * \brief Reset the values of the rectangle.
	 * \param [in] _minX Left.
	 * \param [in] _minY Top.
	 * \param [in] _maxX Right.
	 * \param [in] _maxY Bottom.
	 */
  void Set(float _minX, float _minY, float _maxX, float _maxY);

	void MinX(float _min);
	void MaxX(float _max);
  void MinY(float _min); 
  void MaxY(float _max);

	float MinX(void) const;
	float MaxX(void) const;
	float MinY(void) const;
	float MaxY(void) const;

	const Vector2 &Minimum(void) const;
	const Vector2 &Maximum(void) const;

	float Width(void)  const;
	float Height(void) const;
	Vector2 Size(void) const;

	float Parimeter(void) const;
	float Area(void)      const;

  Vector2 TopLeft(void)     const; //!< minimum x, minimum y.
  Vector2 TopRight(void)    const; //!< maximum x, minimum y.
  Vector2 BottomLeft(void)  const; //!< minimim x, maximum y.
  Vector2 BottomRight(void) const; //!< maximum x, maximum y.

	/**
	 * \brief Checks if the point is within the rectangle.
	 * \param [in] _point The point to query.
	 * \return Returns true if the the point is within the rectangle.
	 */
  bool Contains(const Vector2 &_point) const;

	/**
	 * \brief Checks if the rectangle is fully contained.
	 * \param [in] _rect Rect to query.
	 * \return Returns true if the rectangle is contained within this Rectangle.
	 */
	bool Contains(const Rect &_rect)     const;

	/**
	 * \brief Checks if the rectangles overlap.
	 * \param [in] _rect
	 * \return Returns true if the rectangles overlap.
	 */
  bool Intersects(const Rect &_rect)   const;

	/**
	 * \brief Combines the rectangles to create a rectangle than encompases both.
	 * \param [in] _a
	 * \param [in] _b
	 */
	void Union(const Rect &_a, const Rect &_b);

	/** 
	 * \brief Draws the rectangle.
	 * \param [in] _renderer Renderer to draw to.
	 */
	void Draw(Renderer &_renderer);

public:
	/**
	 * \brief Checks if the rectangles overlap.
	 * \param [in] _rect
	 * \return Returns true if the rectangles overlap.
	 */
	static bool Intersects(const Rect &_a, const Rect &_b);

	/**
	 * \brief Creates a rectangle that encompases both.
	 * \param [in] _a
	 * \param [in] _b
	 * \return Returns the rectangle 
	 */
	static Rect Combine(const Rect &_a, const Rect &_b);
};

#endif //_RECT_H_