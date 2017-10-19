#include "Range.h"

#include "Maths.h"

Range::Range(void) :
	minimum(0.0f),
	maximum(0.0f)
{ }

Range::Range(float _min, float _max) :
	minimum(_min),
	maximum(_max)
{
	Sort();
}

void Range::Set(float _min, float _max)
{
	minimum = _min;
	maximum = _max;
	Sort();
}

void Range::Minimum(float _min)
{
	minimum = _min;
	Sort();
}

void Range::Maximum(float _max)
{
	maximum = _max;
	Sort();
}

bool Range::Contains(float _value) const
{
	return _value >= this->minimum &&
				 _value <= this->maximum;
}

float Range::Distance(float _value) const
{
	if (Contains(_value)) { return 0.0f; }
	return Min(
		Abs(this->minimum - _value),
		Abs(_value - this->maximum)
	);
}

bool Range::Overlaps(const Range &_other) const
{
	return this->minimum <= _other.maximum &&
				 this->maximum >= _other.minimum;
}

float Range::Overlap(const Range &_other) const
{
	return Max(0.0f,
			Min(this->maximum, _other.maximum) - Max(this->minimum, _other.minimum)
	);
}

void Range::Sort(void)
{
	if (minimum > maximum)
	{
		float tmp = maximum;
		maximum = minimum;
		minimum = tmp;
	}
}