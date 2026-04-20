#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"
#include "hittable_list.h"

class triangle : public hittable
{
public:
	triangle(const point3& A, const point3& B, const point3& C, shared_ptr<material> mat)
		: A(A), B(B), C(C), mat(mat) 
	{
		auto n = glm::cross(B - A, C - A);
		normal = glm::normalize(n);
		D = dot(normal, A);
		w = n / dot(n, n);
		set_bounding_box();
	}
private:
};

#endif