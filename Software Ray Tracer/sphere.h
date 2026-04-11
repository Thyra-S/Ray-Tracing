#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class sphere : public hittable
{
public:															// Ensure that the radius is non-negative.
	sphere(const point3& center, float radius) : center(center), radius(std::fmax(0,radius)) {}

	bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override
	{
		auto oc = center - r.origin();
		auto a = glm::dot(r.direction(), r.direction());
		auto h = glm::dot(r.direction(), oc);
		auto c = glm::dot(oc, oc) - radius * radius;
		auto discriminant = h * h - a * c;
		// If the discriminant is negative, the ray misses the sphere.
		if (discriminant < 0)
			return false;

		auto sqrt_discriminant = sqrt(discriminant);

		// Find the nearest root that lies in the acceptable range.
		auto root = (-h - sqrt_discriminant) / a;
		if (root < t_min || root > t_max)
		{
			root = (-h + sqrt_discriminant) / a; // Try the other root.
			if (root < t_min || root > t_max)
				return false; // No roots are in the acceptable range.
		}

		rec.t = root;
		rec.p = r.at(rec.t);
		glm::vec3 outward_normal = (rec.p - center) / radius;
		rec.set_face_normal(r, outward_normal);
		x`	
		return true;
	}
private:
	point3 center;
	float radius;
};

#endif // !SPHERE_H