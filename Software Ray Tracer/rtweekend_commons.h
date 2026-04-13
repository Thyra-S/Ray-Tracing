#ifndef RTWEEKEND__COMMONS_H
#define RTWEEKEND_COMMONS_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <random>


// C++ Std usings

using std::make_shared;
using std::shared_ptr;


// Constants

const float infinity = std::numeric_limits<float>::infinity();
const float pi = 3.1415926535897932385f;

// Utility Functions

inline float degrees_to_radians(float degrees) {
	return degrees * pi / 180.0f;
}

inline float random_float() {
	static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
	static std::mt19937 generator;
	return distribution(generator);
}

inline float random_float(float min, float max) {
	// Returns a random real in [min,max).
	return min + (max - min) * random_float();
}




// Common Headers

#include "color.h"
#include "interval.h"
#include "ray.h"
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

using color = glm::vec3;
using POINT = glm::vec3;

inline glm::vec3 random_unit_vector() {
	return glm::sphericalRand(1.0f);
}

inline glm::vec3 random_on_hemisphere(const glm::vec3& normal) {
	glm::vec3 on_unit_sphere = random_unit_vector();
	if (dot(on_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
		return on_unit_sphere;
	else
		return -on_unit_sphere;
}

#endif
