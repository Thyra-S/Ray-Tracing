#ifndef INTERVAL_H
#define INTERVAL_H

class interval 
{
public:
	float min, max;

	interval() : min(+infinity), max(-infinity) {} // Default interval is empty

	interval(float min, float max) : min(min), max(max) {}

	interval(const interval& a, const interval& b) 
	{
		// Create the interval tightly enclosing the two input intervals.
		min = a.min <= b.min ? a.min : b.min;
		max = a.max >= b.max ? a.max : b.max;
	}

	float size() const 
	{
		return max - min;
	}

	bool contains(float x) const 
	{
		return min <= x && x <= max;
	}

	bool surrounds(float x) const 
	{
		return min < x && x < max;
	}

	float clamp(float x) const 
	{
		if (x < min) return min;
		if (x > max) return max;
		return x;
	}

	interval expand(float delta) const
	{
		auto padding = delta / 2;
		return interval(min - padding, max + padding);
	}

	static const interval empty, universe;
};

const interval interval::empty = interval(+infinity, -infinity);
const interval interval::universe = interval(-infinity, +infinity);

interval operator+(const interval& ival, float displacement) {
	return {ival.min + displacement, ival.max + displacement};
}

interval operator+(float displacement, const interval& ival) {
	return ival + displacement;
}

#endif