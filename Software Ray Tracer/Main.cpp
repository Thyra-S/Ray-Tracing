#include "rtweekend_commons.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

int main()
{
	hittable_list world;

	auto material_ground = make_shared<lambertian>(color(0.8f, 0.8f, 0.0f));
	auto material_center = make_shared<lambertian>(color(0.1f, 0.2f, 0.5f));
	auto material_left = make_shared<dielectric>(1.50f);
	auto material_bubble = make_shared<dielectric>(1.0f / 1.50f);
	auto material_right = make_shared<metal>(color(0.8f, 0.6f, 0.2f), 1.0f);

	world.add(make_shared<sphere>(point3(0.0f, -100.5f, -1.0f), 100.0f, material_ground));
	world.add(make_shared<sphere>(point3(0.0f, 0.0f, -1.2f), 0.5f, material_center));
	world.add(make_shared<sphere>(point3(-1.0f, 0.0f, -1.0f), 0.5f, material_left));
	world.add(make_shared<sphere>(point3(-1.0f, 0.0f, -1.0f), 0.4f, material_bubble));
	world.add(make_shared<sphere>(point3(1.0f, 0.0f, -1.0f), 0.5f, material_right));

	camera cam;

	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 960;
	cam.samples_per_pixel = 4;
	cam.max_depth = 50;
	cam.thread_count = 24;

	cam.vfov = 20;
	cam.lookfrom = point3(-2, 2, 1);
	cam.lookat = point3(0, 0, -1);
	cam.vup = glm::vec3(0, 1, 0);

	cam.render(world);
}