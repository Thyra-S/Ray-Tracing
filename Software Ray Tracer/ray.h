#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>

using point3 = glm::vec3;
class ray {
public:
	ray() : orig(0.0f), dir(0.0f), tm(0.0f) {}

	ray(const point3& origin, const glm::vec3& direction, float time) 
		: orig(origin), dir(direction), tm(time) {}

	ray(const point3& origin, const glm::vec3& direction)
		: ray(origin, direction, 0.0f) {
	}
	const point3& origin() const { return orig; }
	const glm::vec3& direction() const { return dir; }
	float time() const { return tm; }

	point3 at(float t) const {
		return orig + t * dir;
	}

private:
	point3 orig;
	glm::vec3 dir;
	float tm;
};

#endif#pragma once
