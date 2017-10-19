#pragma once

struct Range
{
public:
	float minimum, maximum;

	Range(void);
	Range(float _min, float _max);

	void Set(float _min, float _max);
	void Minimum(float _min);
	void Maximum(float _max);

	bool Contains(float _value) const;
	float Distance(float _value) const;

	bool Overlaps(const Range &_other) const;
	float Overlap(const Range &_other) const;

	void Sort(void);
};
