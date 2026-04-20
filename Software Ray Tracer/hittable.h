#ifndef HITTABLE_H
#define	HITTABLE_H	

#include "aabb.h"

class material;

class hit_record
{
public: 
	point3 p;
	glm::vec3 normal;
	shared_ptr<material> mat;
	float t;
	float u;
	float v;
	bool front_face;
	
	void set_face_normal(const ray& r, const glm::vec3& outward_normal)
	{
		front_face = glm::dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};
class hittable
{
public:
	virtual ~hittable() = default;

	virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;

	virtual aabb bounding_box() const = 0;
};

class translate : public hittable {
public:
	translate(shared_ptr<hittable> object, const glm::vec3& offset)
		: object(object), offset(offset) 
	{
		bbox = object->bounding_box();
	}

	bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
		// Move the ray backwards by the offset
		ray offset_r(r.origin() - offset, r.direction(), r.time());

		// Determine whether an intersection exists along the offset ray (and if so, where)
		if (!object->hit(offset_r, ray_t, rec))
			return false;

		// Move the intersection point forwards by the offset
		rec.p += offset;

		return true;
	}

	aabb bounding_box() const override { return bbox; }

private:
	shared_ptr<hittable> object;
	glm::vec3 offset;
	aabb bbox;
};
#endif // !HITTABLE_H
