#include "rtweekend_commons.h"

#include "bvh.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "texture.h"

int main()
{
	hittable_list world;

	auto checker = make_shared<checker_texture>(0.32f, color(0.2f, 0.3f, 0.1f), color(0.9f, 0.9f, 0.9f));
	world.add(make_shared<sphere>(point3(0.0f, -1000.0f, 0.0f), 1000.0f, make_shared<lambertian>(checker)));

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			auto choose_mat = random_float();
			point3 center(a + 0.9f * random_float(), 0.2f, b + 0.9f * random_float());

			if ((center - point3(4, 0.2f, 0)).length() > 0.9f) {
				shared_ptr<material> sphere_material;

				if (choose_mat < 0.8f) {
					// diffuse
					auto albedo = glm::vec3(random_float(), random_float(), random_float()) 
								* glm::vec3(random_float(),random_float(),random_float());
					sphere_material = make_shared<lambertian>(albedo);
					auto center2 = center + glm::vec3(0, random_float(0,0.5f), 0);
					world.add(make_shared<sphere>(center,center2, 0.2f, sphere_material));
				}
				else if (choose_mat < 0.95f) {
					// metal
					auto albedo = color(random_float(0.5f, 1.0f), random_float(0.5f, 1.0f), random_float(0.5f, 1.0f));
					auto fuzz = random_float(0.0f, 0.5f);
					sphere_material = make_shared<metal>(albedo, fuzz);
					world.add(make_shared<sphere>(center, 0.2f, sphere_material));
				}
				else {
					// glass
					sphere_material = make_shared<dielectric>(1.5f);
					world.add(make_shared<sphere>(center, 0.2f, sphere_material));
				}
			}
		}
	}

	auto material1 = make_shared<dielectric>(1.5f);
	world.add(make_shared<sphere>(point3(0.0f, 1.0f, 0.0f), 1.0f, material1));

	auto material2 = make_shared<lambertian>(color(0.4f, 0.2f, 0.1f));
	world.add(make_shared<sphere>(point3(-4.0f, 1.0f, 0.0f), 1.0f, material2));

	auto material3 = make_shared<metal>(color(0.7f, 0.6f, 0.5f), 0.0f);
	world.add(make_shared<sphere>(point3(4.0f, 1.0f, 0.0f), 1.0f, material3));

	world = hittable_list(make_shared<bvh_node>(world));

	camera cam;

	cam.aspect_ratio	  = 16.0f / 9.0f;
	cam.image_width		  = 960;
	cam.samples_per_pixel = 64;
	cam.max_depth		  = 50;
	cam.thread_count	  = 12;

	cam.vfov	 = 20;
	cam.lookfrom = point3(13, 2, 3);
	cam.lookat	 = point3(0, 0, 0);
	cam.vup		 = glm::vec3(0, 1, 0);

	cam.defocus_angle = 0.6f;
	cam.focus_dist	  = 10.0f;

	cam.render(world);
}