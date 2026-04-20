#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class sphere : public hittable
{
public:															// Ensure that the radius is non-negative.
	sphere(const point3& static_center, float radius, shared_ptr<material> mat) 
		: center(static_center, glm::vec3(0,0,0)), radius(std::fmaxf(0.0f, radius)), mat(mat) 
	{
		auto rvec = glm::vec3(radius, radius, radius);
		bbox = aabb(static_center - rvec, static_center + rvec);
	}

	sphere(const point3& center1, const point3& center2, float radius, shared_ptr<material> mat)
		: center(center1, center2-center1), radius(std::fmaxf(0.0f, radius)), mat(mat) 
	{
		auto rvec = glm::vec3(radius, radius, radius);
		aabb box1(center.at(0) - rvec, center.at(0) + rvec);
		aabb box2(center.at(1) - rvec, center.at(1) + rvec);
		bbox = aabb(box1, box2);
	}

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
		auto root = (h - sqrt_discriminant) / a;
		if (!ray_t.surrounds(root))
		{
			root = (h + sqrt_discriminant) / a; // Try the other root.
			if (!ray_t.surrounds(root))
				return false; // No roots are in the acceptable range.
		}

		rec.t = root;
		rec.p = r.at(rec.t);
		glm::vec3 outward_normal = (rec.p - current_center) / radius;
		rec.set_face_normal(r, outward_normal);
		get_sphere_uv(outward_normal, rec.u, rec.v);
		rec.mat = mat;
		
		return true;
	}

	aabb bounding_box() const override { return bbox; }

private:
	ray center;
	float radius;
	shared_ptr<material> mat;
	aabb bbox;

	static void get_sphere_uv(const point3& p, float& u, float& v)
	{
		auto theta = acos(-p.y);
		auto phi = atan2(-p.z, p.x) + pi;

		u = phi / (2*pi);
		v = theta / pi;
	}
};

#endif // !SPHERE_H