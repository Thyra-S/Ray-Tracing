#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>

using point3 = glm::vec3;
class ray {
public:
	ray() 
	{
		orig = point3(0, 0, 0);
		dir = glm::vec3(0, 0, 0);
	}

	ray(const point3& origin, const glm::vec3& direction) : orig(origin), dir(direction) {}

	const point3& origin() const { return orig; }
	const glm::vec3& direction() const { return dir; }

	point3 at(float t) const {
		return orig + t * dir;
	}

private:
	point3 orig;
	glm::vec3 dir;
};

#endif#pragma once
