#include "rtweekend_commons.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

int main()
{
	hittable_list world;

	auto material_ground = make_shared<lambertian>(color(1.0f, 1.0f, 1.0f));
	auto material_lw = make_shared<lambertian>(color(1.0f, 0.0f, 0.0f));
	auto material_rw = make_shared<lambertian>(color(0.0f, 1.0f, 0.0f));
	// auto material_center = make_shared<lambertian>(color(0.1f, 0.2f, 0.5f));
	auto material_left = make_shared<metal>(color(0.8f, 0.8f, 0.8f));
	auto material_right = make_shared<metal>(color(0.8f, 0.6f, 0.2f));

	world.add(make_shared<sphere>(point3(0.0f, -100.5f, -1.0f), 100.0f, material_ground));
	world.add(make_shared<sphere>(point3(52.5f, 0.0f, -1.0f), 50.0f, material_lw));
	world.add(make_shared<sphere>(point3(-52.5f, 0.0f, -1.0f), 50.0f, material_rw));
	//world.add(make_shared<sphere>(point3(0.0f, 0.0f, -1.2f), 0.5f, material_center));
	world.add(make_shared<sphere>(point3(-1.0f, 0.0f, -1.0f), 0.5f, material_left));
	world.add(make_shared<sphere>(point3(1.0f, 0.0f, -1.0f), 0.5f, material_right));

	camera cam;

	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 1280;
	cam.samples_per_pixel = 512;
	cam.max_depth = 50;
	cam.thread_count = 24;

	cam.render(world);
}