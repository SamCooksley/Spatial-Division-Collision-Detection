#ifndef _RANGE_H_
#define _RANGE_H_

#include "Maths.h"

/**
 * \brief Define a range or section.
 */

class Range
{
 public:
  float min, max;

	Range(); //!< Default constructor.
	Range(float _min, float _max); //!< Constructor.

  void Sort(); //!< Sort the values so min will be the smaller value and max will be the higher value.

  bool Contains(float _x) const; //!< Is the point within the range.
	float Distance(float _x) const; //!< How far is the point from the range.

  bool Overlaps(const Range& _other) const; //!< Do the ranges overlap.
  float Overlap(const Range& _other) const; //!< Get the amount of overlap between this and the other range.
  float Distance(const Range& _other) const; //!< Get the distance between this and the other range.

  static bool Overlaps(const Range& _a, const Range& _b); //!< Do the ranges overlap.
  static float Overlap(const Range& _a, const Range& _b); //!< Get the amount of overlap between two ranges.
  static float Distance(const Range& _a, const Range& _b); //!< Get the distance between two ranges.
};

#endif //_RANGE_H_