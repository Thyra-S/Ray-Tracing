#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class sphere : public hittable
{
public:															// Ensure that the radius is non-negative.
	sphere(const point3& static_center, float radius, shared_ptr<material> mat) 
		: center(static_center, glm::vec3(0,0,0)), radius(std::fmaxf(0.0f, radius)), mat(mat) {}

	sphere(const point3& center1, const point3& center2, float radius, shared_ptr<material> mat)
		: center(center1, center2-center1), radius(std::fmaxf(0.0f, radius)), mat(mat) {}

	bool hit(const ray& r, interval ray_t, hit_record& rec) const override
	{
		point3 current_center = center.at(r.time());
		glm::vec3 oc = current_center - r.origin();
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
		if (!ray_t.surrounds(root))
		{
			root = (-h + sqrt_discriminant) / a; // Try the other root.
			if (!ray_t.surrounds(root))
				return false; // No roots are in the acceptable range.
		}

		rec.t = root;
		rec.p = r.at(rec.t);
		glm::vec3 outward_normal = (rec.p - current_center) / radius;
		rec.set_face_normal(r, outward_normal);
		rec.mat = mat;
		
		return true;
	}
private:
	ray center;
	float radius;
	shared_ptr<material> mat;
};

#endif // !SPHERE_H