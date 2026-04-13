#ifndef RTWEEKEND__COMMONS_H
#define RTWEEKEND_COMMONS_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>


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

// Common Headers

#include "color.h"
#include "ray.h"
#include <glm/glm.hpp>

#endif
