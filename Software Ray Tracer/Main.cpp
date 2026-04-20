#include "rtweekend_commons.h"
#include "bvh.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "quad.h"
#include "texture.h"

#define THREAD_COUNT 12

void bouncing_spheres()
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
	cam.thread_count	  = THREAD_COUNT;
	cam.background = color(0.70, 0.80, 1.00);

	cam.vfov	 = 20.0f;
	cam.lookfrom = point3(13.0f, 2.0f, 3.0f);
	cam.lookat = point3(0.0f, 0.0f, 0.0f);
	cam.vup = glm::vec3(0.0f, 1.0f, 0.0f);

	cam.defocus_angle = 0.6f;
	cam.focus_dist	  = 10.0f;

	cam.render(world);
}

void checkered_spheres() {
	hittable_list world;

	auto checker = make_shared<checker_texture>(0.03f, color(0.2f, 0.3f, 0.1f), color(0.9f, 0.9f, 0.9f));

	world.add(make_shared<sphere>(point3(0, -10, 0), 10, make_shared<lambertian>(checker)));
	world.add(make_shared<sphere>(point3(0, 10, 0), 10, make_shared<lambertian>(checker)));

	camera cam;

	cam.aspect_ratio = 16.0f / 9.0f;
	cam.image_width = 640;
	cam.samples_per_pixel = 100;
	cam.max_depth = 50;
	cam.thread_count = THREAD_COUNT;
	cam.background = color(0.70, 0.80, 1.00);

	cam.vfov = 20.0f;
	cam.lookfrom = point3(13.0f, 2.0f, 3.0f);
	cam.lookat = point3(0.0f, 0.0f, 0.0f);
	cam.vup = glm::vec3(0.0f, 1.0f, 0.0f);

	cam.defocus_angle = 0.0f;
	cam.focus_dist = 10.0f;

	cam.render(world);
}

void earth() {
	auto earth_texture = make_shared<image_texture>("images/earthmap.jpg");
	auto earth_surface = make_shared<lambertian>(earth_texture);
	auto globe = make_shared<sphere>(point3(0.0f, 0.0f, 0.0f), 2.0f, earth_surface);

	camera cam;

	cam.aspect_ratio = 16.0f / 9.0f;
	cam.image_width = 400;
	cam.samples_per_pixel = 100;
	cam.max_depth = 50;
	cam.thread_count = THREAD_COUNT;
	cam.background = color(0.70, 0.80, 1.00);

	cam.vfov = 20.0f;
	cam.lookfrom = point3(0.0f, 0.0f, 12.0f);
	cam.lookat = point3(0.0f, 0.0f, 0.0f);
	cam.vup = glm::vec3(0.0f, 1.0f, 0.0f);

	cam.defocus_angle = 0.0f;

	cam.render(hittable_list(globe));
}

void perlin_spheres() {
	hittable_list world;

	auto pertext = make_shared<noise_texture>(4.0f);
	world.add(make_shared<sphere>(point3(0.0f, -1000.0f, 0.0f), 1000.0f, make_shared<lambertian>(pertext)));
	world.add(make_shared<sphere>(point3(0.0f, 2.0f, 0.0f), 2.0f, make_shared<lambertian>(pertext)));

	camera cam;

	cam.aspect_ratio = 16.0f / 9.0f;
	cam.image_width = 400;
	cam.samples_per_pixel = 100;
	cam.max_depth = 50;
	cam.thread_count = THREAD_COUNT;
	cam.background = color(0.70, 0.80, 1.00);

	cam.vfov = 20.0f;
	cam.lookfrom = point3(13.0f, 2.0f, 3.0f);
	cam.lookat = point3(0.0f, 0.0f, 0.0f);
	cam.vup = glm::vec3(0.0f, 1.0f, 0.0f);

	cam.defocus_angle = 0.0f;

	cam.render(world);
}

void quads() {
	hittable_list world;

	// Materials
	auto left_red = make_shared<lambertian>(color(1.0f, 0.2f, 0.2f));
	auto back_green = make_shared<lambertian>(color(0.2f, 1.0f, 0.2f));
	auto right_blue = make_shared<lambertian>(color(0.2f, 0.2f, 1.0f));
	auto upper_orange = make_shared<lambertian>(color(1.0f, 0.5f, 0.0f));
	auto lower_teal = make_shared<lambertian>(color(0.2f, 0.8f, 0.8f));

	// Quads
	world.add(make_shared<quad>(point3(-3.0f, -2.0f, 5.0f), glm::vec3(0.0f, 0.0f, -4.0f), glm::vec3(0.0f, 4.0f, 0.0f), left_red));
	world.add(make_shared<quad>(point3(-2.0f, -2.0f, 0.0f), glm::vec3(4.0f, 0.0f, 0.0f), glm::vec3(0.0f, 4.0f, 0.0f), back_green));
	world.add(make_shared<quad>(point3(3.0f, -2.0f, 1.0f), glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f, 4.0f, 0.0f), right_blue));
	world.add(make_shared<quad>(point3(-2.0f, 3.0f, 1.0f), glm::vec3(4.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 4.0f), upper_orange));
	world.add(make_shared<quad>(point3(-2.0f, -3.0f, 5.0f), glm::vec3(4.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -4.0f), lower_teal));

	camera cam;

	cam.aspect_ratio = 1.0f;
	cam.image_width = 400;
	cam.samples_per_pixel = 100;
	cam.max_depth = 50;
	cam.thread_count = THREAD_COUNT;
	cam.background = color(0.70, 0.80, 1.00);

	cam.vfov = 80.0f;
	cam.lookfrom = point3(0.0f, 0.0f, 9.0f);
	cam.lookat = point3(0.0f, 0.0f, 0.0f);
	cam.vup = glm::vec3(0.0f, 1.0f, 0.0);

	cam.defocus_angle = 0.0f;

	cam.render(world);
}

void simple_light() {
	hittable_list world;

	auto pertext = make_shared<noise_texture>(4);
	world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
	world.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(pertext)));

	auto difflight = make_shared<diffuse_light>(color(4, 4, 4));
	world.add(make_shared<quad>(point3(3, 1, -2), glm::vec3(2, 0, 0), glm::vec3(0, 2, 0), difflight));

	camera cam;

	cam.aspect_ratio = 16.0f / 9.0f;
	cam.image_width = 400;
	cam.samples_per_pixel = 100;
	cam.max_depth = 50;
	cam.thread_count = THREAD_COUNT;
	cam.background = color(0, 0, 0);

	cam.vfov = 20;
	cam.lookfrom = point3(26, 3, 6);
	cam.lookat = point3(0, 2, 0);
	cam.vup = glm::vec3(0, 1, 0);

	cam.defocus_angle = 0;

	cam.render(world);
}

void cornell_box() {
	hittable_list world;

	auto red = make_shared<lambertian>(color(.65, .05, .05));
	auto white = make_shared<lambertian>(color(.73, .73, .73));
	auto green = make_shared<lambertian>(color(.12, .45, .15));
	auto light = make_shared<diffuse_light>(color(15, 15, 15));

	world.add(make_shared<quad>(point3(555, 0, 0), glm::vec3(0, 555, 0), glm::vec3(0, 0, 555), green));
	world.add(make_shared<quad>(point3(0, 0, 0), glm::vec3(0, 555, 0), glm::vec3(0, 0, 555), red));
	world.add(make_shared<quad>(point3(343, 554, 332), glm::vec3(-130, 0, 0), glm::vec3(0, 0, -105), light));
	world.add(make_shared<quad>(point3(0, 0, 0), glm::vec3(555, 0, 0), glm::vec3(0, 0, 555), white));
	world.add(make_shared<quad>(point3(555, 555, 555), glm::vec3(-555, 0, 0), glm::vec3(0, 0, -555), white));
	world.add(make_shared<quad>(point3(0, 0, 555), glm::vec3(555, 0, 0), glm::vec3(0, 555, 0), white));

	world.add(box(point3(130, 0, 65), point3(295, 165, 230), white));
	world.add(box(point3(265, 0, 295), point3(430, 330, 460), white));

	camera cam;

	cam.aspect_ratio = 1.0;
	cam.image_width = 600;
	cam.samples_per_pixel = 200;
	cam.max_depth = 50;
	cam.thread_count = THREAD_COUNT;
	cam.background = color(0, 0, 0);

	cam.vfov = 40;
	cam.lookfrom = point3(278, 278, -800);
	cam.lookat = point3(278, 278, 0);
	cam.vup = glm::vec3(0, 1, 0);

	cam.defocus_angle = 0;

	cam.render(world);
}

int main()
{
	switch (7)
	{
		case 1: bouncing_spheres(); break; 
		case 2: checkered_spheres(); break;
		case 3: earth(); break;
		case 4: perlin_spheres(); break;
		case 5: quads(); break;
		case 6: simple_light(); break;
		case 7: cornell_box(); break;
	}
	
}