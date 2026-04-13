#ifndef COLOR_H
#define COLOR_H

#include "interval.h"
#include <glm.hpp>

using color = glm::vec3;

inline float linear_to_gamma(float linear) {
	if(linear > 0)
		return std::sqrt(linear);

	return 0;
}
void write_color(std::ostream& out, const color& pixel_color) 
{
	auto r = pixel_color.r;
	auto g = pixel_color.g;
	auto b = pixel_color.b;

	r = linear_to_gamma(r);
	g = linear_to_gamma(g);
	b = linear_to_gamma(b);

	// Translate the [0,1] component values to the byte range [0,255].
	static const interval intensity(0.000f, 0.999f);
	int rbyte = int(256 * intensity.clamp(r));
	int gbyte = int(256 * intensity.clamp(g));
	int bbyte = int(256 * intensity.clamp(b));

	// Write out the pixel color components.
	out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif#pragma once
