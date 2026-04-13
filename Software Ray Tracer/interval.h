#ifndef INTERVAL_H
#define INTERVAL_H

class interval {
public:
	float min, max;

	interval() : min(+infinity), max(-infinity) {} // Default interval is empty

	interval(float min, float max) : min(min), max(max) {}

	float size() const {
		return max - min;
	}

	bool contains(float x) const {
		return min <= x && x <= max;
	}

	bool surrounds(float x) const {
		return min < x && x < max;
	}

	static const interval empty, universe;
};

const interval interval::empty = interval(+infinity, -infinity);
const interval interval::universe = interval(-infinity, +infinity);

#endif