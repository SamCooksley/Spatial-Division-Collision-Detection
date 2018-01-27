#include "Range.h"

Range::Range() :
	min(.0f), max(.0f)
{ }

Range::Range(float _min, float _max) :
	min(_min), max(_max)
{ }

void Range::Sort()
{
  if (min > max)
  {
    float tmp = min;
    min = max;
    max = tmp;
  }
}

bool Range::Contains(float _x) const
{
	return _x >= this->min &&
				 _x <= this->max;
}

float Range::Distance(float _x) const
{
	if (Contains(_x)) { return .0f; }
	return Min(
    Abs(this->min - _x),
    Abs(_x - this->max)
  );
}

bool Range::Overlaps(const Range& _other) const
{
  return Overlaps(*this, _other);
}

float Range::Overlap(const Range& _other) const
{
  return Overlap(*this, _other);
}

float Range::Distance(const Range& _other) const
{
  return Distance(*this, _other);
}

bool Range::Overlaps(const Range& _a, const Range& _b)
{
  return _a.min <= _b.max &&
         _a.max >= _b.min;
}

float Range::Overlap(const Range& _a, const Range& _b)
{
  return Max(0.0f,
    Min(_a.max, _b.max) - Max(_a.min, _b.min)
  );
}

float Range::Distance(const Range& _a, const Range& _b)
{
  if (Overlaps(_a, _b)) { return .0f; }
  return Min(Abs(_a.min - _b.max), Abs(_b.min - _a.max));
}