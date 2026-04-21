#ifndef BVH_H
#define BVH_H

#include "aabb.h"
#include "hittable.h"
#include "hittable_list.h"

#include <algorithm>

struct Bin {
	int count = 0;
	aabb bounds = aabb::empty;
};

class bvh_node : public hittable
{
public:
	bvh_node(hittable_list list) : bvh_node(list.objects, 0, list.objects.size()) 
	{} // Note this temporarily creates a copy of the list and then deletes it

	bvh_node(std::vector<shared_ptr<hittable>>& objects, size_t start, size_t end)
	{
		size_t object_span = end - start;
		// Build a bounding box to include all nodes
		bbox = aabb::empty;
		aabb centroid_bbox = aabb::empty;

		for (size_t object_index = start; object_index < end; object_index++)
		{
			aabb obj_box = objects[object_index]->bounding_box();
			bbox = aabb(bbox, obj_box);

			point3 c = obj_box.center();
			centroid_bbox = aabb(centroid_bbox, aabb(c, c));
		}

		int best_axis = 0;
		int best_split_index = -1;
		float min_cost = infinity;
		const int num_bins = 12;

		// Evaluate splits using the Surface Area Heuristic (SAH) for each axis and binning configuration

		for (int axis = 0; axis < 3; axis++)
		{
			float bounds_min = centroid_bbox.axis_interval(axis).min;
			float bounds_max = centroid_bbox.axis_interval(axis).max;
			float bounds_size = bounds_max - bounds_min;

			// Skip this axis if all object centers are perfectly aligned on it
			if (bounds_size < 0.00001f) continue;

			// Use stack arrays instead of std::vector for massive speed gains
			Bin bins[num_bins];

			for (size_t object_index = start; object_index < end; object_index++)
			{
				aabb obj_box = objects[object_index]->bounding_box();
				point3 c = obj_box.center();


				float normalized_pos = (c[axis] - bounds_min) / bounds_size;

				auto bin_index = static_cast<int>(num_bins * normalized_pos);

				// Clamp safety bounds
				if (bin_index == num_bins) bin_index = num_bins - 1;
				if (bin_index < 0) bin_index = 0;

				bins[bin_index].count++;
				bins[bin_index].bounds = aabb(bins[bin_index].bounds, obj_box);
			}

			// Stack arrays for sweeps
			float left_areas[num_bins];
			float right_areas[num_bins];
			int left_counts[num_bins];
			int right_counts[num_bins];

			aabb left_bbox = aabb::empty;
			int left_count = 0;

			for (int i = 0; i < num_bins; i++)
			{
				left_bbox = aabb(left_bbox, bins[i].bounds);
				left_areas[i] = left_bbox.surface_area();
				left_count += bins[i].count;
				left_counts[i] = left_count;
			}

			aabb right_bbox = aabb::empty;
			int right_count = 0;

			for (int i = num_bins - 1; i >= 0; i--)
			{
				right_bbox = aabb(right_bbox, bins[i].bounds);
				right_areas[i] = right_bbox.surface_area();
				right_count += bins[i].count;
				right_counts[i] = right_count;
			}

			// Evaluate SAH cost
			for (int i = 0; i < num_bins - 1; i++)
			{
				// SAH base cost (1.0f) + relative cost of the split
				float cost = 1.0f + (left_counts[i] * left_areas[i] + right_counts[i + 1] * right_areas[i + 1]);
				if (cost < min_cost)
				{
					min_cost = cost;
					best_axis = axis;
					best_split_index = i;
				}
			}
		}
		// Partition
		if (best_split_index == -1 || object_span <= 2)
			if (object_span == 1)
				left = right = objects[start];
			else if (object_span == 2)
			{
				left = objects[start];
				right = objects[start + 1];
			}
			else
			{
				int axis = bbox.longest_axis();
				if (axis == 0)
					std::sort(objects.begin() + start, objects.begin() + end, box_x_compare);
				else if (axis == 1)
					std::sort(objects.begin() + start, objects.begin() + end, box_y_compare);
				else
					std::sort(objects.begin() + start, objects.begin() + end, box_z_compare);

				auto mid = start + object_span / 2;

				left = make_shared<bvh_node>(objects, start, mid);
				right = make_shared<bvh_node>(objects, mid, end);
			}
		else
		{	// Partition based on best split
			auto mid_iterator = std::partition(objects.begin() + start, objects.begin() + end, 
						[best_axis,
						bounds_min = centroid_bbox.axis_interval(best_axis).min,
						bounds_size = centroid_bbox.axis_interval(best_axis).size(),
						num_bins,
						best_split_index]
						(const shared_ptr<hittable>& obj)
				{
				aabb obj_box = obj->bounding_box();
				point3 c = obj_box.center();
				float normalized_pos = (c[best_axis] - bounds_min) / bounds_size;
				auto bin_index = static_cast<int>(num_bins * normalized_pos);
				if (bin_index == num_bins) bin_index = num_bins - 1;
				if (bin_index < 0) bin_index = 0;
				return bin_index <= best_split_index;
				});

			int mid = std::distance(objects.begin(), mid_iterator);

			if (mid == start || mid == end) // Partition failed, fallback to midpoint split
			{
				mid = start + object_span / 2;
			}

			left = make_shared<bvh_node>(objects, start, mid);
			right = make_shared<bvh_node>(objects, mid, end);
		}
	}

	bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
		if (!bbox.hit(r, ray_t))
			return false;

		bool hit_left = left->hit(r, ray_t, rec);
		bool hit_right = right->hit(r, interval(ray_t.min, hit_left ? rec.t : ray_t.max), rec);

		return hit_left || hit_right;
	}

	aabb bounding_box() const override { return bbox; }

private:
	shared_ptr<hittable> left;
	shared_ptr<hittable> right;
	aabb bbox;

	static bool box_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis_index) 
	{
		auto a_axis_interval = a->bounding_box().axis_interval(axis_index);
		auto b_axis_interval = b->bounding_box().axis_interval(axis_index);
		return a_axis_interval.min < b_axis_interval.min;
	}

	static bool box_x_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
		return box_compare(a, b, 0);
	}

	static bool box_y_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
		return box_compare(a, b, 1);
	}

	static bool box_z_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
		return box_compare(a, b, 2);
	}
};

#endif