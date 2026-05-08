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

inline float degrees_to_radians(float degrees) 
{
	return degrees * pi / 180.0f;
}

inline float random_float() 
{
	static thread_local std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
	static thread_local std::mt19937 generator(std::random_device{}());
	return distribution(generator);
}

inline float random_float(float min, float max) 
{
	// Returns a random real in [min,max).
	return min + (max - min) * random_float();
}

inline int random_int(int min, int max) {
    // Returns a random integer in [min,max].
	return int(random_float(min, max + 1));
}




// Common Headers

#include "color.h"
#include "interval.h"
#include "ray.h"

#define GLM_FORCE_INTRINSICS // Tells GLM to use SIMD instructions
#define GLM_FORCE_AVX2       // Tells GLM to use AVX2 specifically
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>



using color = glm::vec3;
using point = glm::vec3;

inline glm::vec3 random_unit_vector() 
{
	while (true)
	{
		auto p = glm::vec3(random_float(-1.0f, 1.0f), random_float(-1.0f, 1.0f), random_float(-1.0f, 1.0f));
		if (1e-16f < glm::dot(p, p) && glm::dot(p, p) <= 1.0f)
			return glm::normalize(p);
	}
}

inline glm::vec3 random_on_hemisphere(const glm::vec3& normal) 
{
	glm::vec3 on_unit_sphere = random_unit_vector();
	if (dot(on_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
		return on_unit_sphere;
	else
		return -on_unit_sphere;
}

inline glm::vec3 random_cosine_direction() {
	auto r1 = random_float();
	auto r2 = random_float();

	auto phi = 2 * pi * r1;
	auto x = std::cos(phi) * std::sqrt(r2);
	auto y = std::sin(phi) * std::sqrt(r2);
	auto z = std::sqrt(1 - r2);

	return { x, y, z };
}

bool near_zero(glm::vec3 vec) 
{
	// Return true if the vector is close to zero in all dimensions.
	auto s = 1e-8;
	return (std::fabs(vec.x) < s) && (std::fabs(vec.y) < s) && (std::fabs(vec.z) < s);
}

inline glm::vec3 reflect(const glm::vec3& v, const glm::vec3& n) 
{
	return v - 2.0f * dot(v, n) * n;
}

inline glm::vec3 refract(const glm::vec3& uv, const glm::vec3& n, float etai_over_etat) 
{
	auto cos_theta = std::fminf(glm::dot(-uv, n), 1.0);
	glm::vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
	glm::vec3 r_out_parallel = -std::sqrt(std::fabsf(1.0f - glm::dot(r_out_perp,r_out_perp))) * n;
	return r_out_perp + r_out_parallel;
}

inline glm::vec3 random_in_unit_disk() 
{
	while (true) 
	{
		auto p = glm::vec3(random_float(-1, 1), random_float(-1, 1), 0);
		if (glm::dot(p, p) < 1)
			return p;
	}
}

#endif
