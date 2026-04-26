#ifndef MESH_H
#define MESH_H

class mesh : public hittable
{
public:
	mesh() {}

	mesh(const hittable_list& triangles) 
	{
		// Wrap all the triangles for this specific shape into a BVH node
		bvh = make_shared<bvh_node>(triangles);
	}

	virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const override 
	{
		// The mesh just delegates the hit test to its internal BVH
		return bvh->hit(r, ray_t, rec);
	}

	aabb bounding_box() const override 
	{
		return bvh->bounding_box();
	}
private:
	shared_ptr<hittable> bvh;
};


#endif