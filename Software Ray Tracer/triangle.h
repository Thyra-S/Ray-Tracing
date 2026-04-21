#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"
#include "hittable_list.h"

class triangle : public hittable
{
public:

	triangle(const point3& Q, const glm::vec3& u, const glm::vec3& v, shared_ptr<material> mat)
		: Q(Q), u(u), v(v), mat(mat) 
	{
		auto n = glm::cross(u, v);

		normal = glm::normalize(n);
		D = dot(normal, Q);
		w = n / dot(n, n);

		set_bounding_box();
	}


	virtual void set_bounding_box() 
	{
		// Compute the bounding box of all three vertices.
		auto bbox_diagonal1 = aabb(Q, Q + u);
		auto bbox_diagonal2 = aabb(Q + u, Q + v);
		auto bbox_diagonal3 = aabb(Q, Q + v);
		bbox = aabb(aabb(bbox_diagonal1, bbox_diagonal2), bbox_diagonal3);
	}

	aabb bounding_box() const override { return bbox; }

	bool hit(const ray& r, interval ray_t, hit_record& rec) const override
	{
		auto denom = glm::dot(normal, r.direction());

		// If the ray is parallel to the plane of the triangle, there is no hit.
		if (std::fabs(denom) < 1e-8)
			return false;

		// Return false if the hit point parameter t is outside the ray interval.
		auto t = (D - glm::dot(normal, r.origin())) / denom;
		if (!ray_t.contains(t))
			return false;

		// Determine if the hit point lies within the planar shape using its plane coordinates.
		auto intersection = r.at(t);
		glm::vec3 planar_hitpt_vector = intersection - Q;
		auto alpha = dot(w, cross(planar_hitpt_vector, v));
		auto beta = dot(w, cross(u, planar_hitpt_vector));

		if (!is_interior(alpha, beta, rec))
			return false;

		// Ray hits the 2D shape; set the rest of the hit record and return true.
		rec.t = t;
		rec.p = r.at(t);
		rec.mat = mat;
		rec.set_face_normal(r, normal);

		return true;
	}

	virtual bool is_interior(float a, float b, hit_record& rec) const
	{
		// if the hit point is outside the triangle, then either a or b is negative, or their sum is greater than 1.
		if (a < 0 || b < 0 || a + b > 1)
			return false;

		rec.u = a;
		rec.v = b;
		return true;
	}

private:
	point3 Q;
	glm::vec3 u, v, w, normal;
	float D;
	shared_ptr<material> mat;
	aabb bbox;
};

inline shared_ptr<hittable> make_triangle(const point3& A, const point3& B, const point3& C, shared_ptr<material> mat) 
{
	return make_shared<triangle>(A, B - A, C - A, mat);
}

inline shared_ptr<hittable> tetrahedron(const point3& A, const point3& B, const point3& C, const point3& D, shared_ptr<material> mat)
{
	auto list = make_shared<hittable_list>();

	// Calculate the center (centroid) of the tetrahedron
	point3 center = (A + B + C + D) / 4.0f;

	// Helper lambda to automatically fix the winding order
	auto add_outward_face = [&](const point3& v0, const point3& v1, const point3& v2) {
		glm::vec3 u = v1 - v0;
		glm::vec3 v = v2 - v0;
		glm::vec3 n = glm::cross(u, v);

		glm::vec3 center_to_face = v0 - center;

		// If the normal points towards the center, swap v1 and v2 to flip the normal
		if (glm::dot(n, center_to_face) < 0) {
			list->add(make_triangle(v0, v2, v1, mat));
		}
		else {
			list->add(make_triangle(v0, v1, v2, mat));
		}
		};

	add_outward_face(A, B, C);
	add_outward_face(A, C, D);
	add_outward_face(A, D, B);
	add_outward_face(B, D, C);

	return list;
}
#endif