#include "rtweekend_commons.h"
#include "bvh.h"
#include "camera.h"
#include "constant_medium.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "quad.h"
#include "triangle.h"
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

void tris() {
	hittable_list world;

	// Materials
	auto left_red = make_shared<lambertian>(color(1.0f, 0.2f, 0.2f));
	auto back_green = make_shared<lambertian>(color(0.2f, 1.0f, 0.2f));
	auto right_blue = make_shared<lambertian>(color(0.2f, 0.2f, 1.0f));
	auto upper_orange = make_shared<lambertian>(color(1.0f, 0.5f, 0.0f));
	auto lower_teal = make_shared<lambertian>(color(0.2f, 0.8f, 0.8f));

	// Quads
	world.add(make_shared<triangle>(point3(-3.0f, -2.0f, 5.0f), glm::vec3(0.0f, 0.0f, -4.0f), glm::vec3(0.0f, 4.0f, 0.0f), left_red));
	world.add(make_shared<triangle>(point3(-3.0f, 2.0f, 1.0f), glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f, -4.0f, 0.0f), right_blue));

	world.add(make_shared<triangle>(point3(-2.0f, -2.0f, 0.0f), glm::vec3(4.0f, 0.0f, 0.0f), glm::vec3(0.0f, 4.0f, 0.0f), back_green));
	world.add(make_shared<triangle>(point3(2.0f, 2.0f, 0.0f), glm::vec3(-4.0f, 0.0f, 0.0f), glm::vec3(0.0f, -4.0f, 0.0f), upper_orange));

	world.add(make_shared<triangle>(point3(3.0f, -2.0f, 1.0f), glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f, 4.0f, 0.0f), right_blue));
	world.add(make_shared<triangle>(point3(3.0f, 2.0f, 5.0f), glm::vec3(0.0f, 0.0f, -4.0f), glm::vec3(0.0f, -4.0f, 0.0f), left_red));

	world.add(make_shared<triangle>(point3(-2.0f, 3.0f, 1.0f), glm::vec3(4.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 4.0f), upper_orange));
	world.add(make_shared<triangle>(point3(2.0f, 3.0f, 5.0f), glm::vec3(-4.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -4.0f), lower_teal));

	world.add(make_shared<triangle>(point3(-2.0f, -3.0f, 5.0f), glm::vec3(4.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -4.0f), lower_teal));
	world.add(make_shared<triangle>(point3(2.0f, -3.0f, 1.0f), glm::vec3(-4.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 4.0f), back_green));

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

	shared_ptr<hittable> box1 = box(point3(0, 0, 0), point3(165, 330, 165), white);
	box1 = make_shared<rotate_y>(box1, 15);
	box1 = make_shared<translate>(box1, glm::vec3(265, 0, 295));
	world.add(box1);

	shared_ptr<hittable> box2 = box(point3(0, 0, 0), point3(165, 165, 165), white);
	box2 = make_shared<rotate_y>(box2, -18);
	box2 = make_shared<translate>(box2, glm::vec3(130, 0, 65));
	world.add(box2);

	camera cam;

	cam.aspect_ratio = 1.0f;
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

void cornell_smoke() {
	hittable_list world;

	auto red = make_shared<lambertian>(color(.65, .05, .05));
	auto white = make_shared<lambertian>(color(.73, .73, .73));
	auto green = make_shared<lambertian>(color(.12, .45, .15));
	auto light = make_shared<diffuse_light>(color(7, 7, 7));

	world.add(make_shared<quad>(point3(555, 0, 0), glm::vec3(0, 555, 0), glm::vec3(0, 0, 555), green));
	world.add(make_shared<quad>(point3(0, 0, 0), glm::vec3(0, 555, 0), glm::vec3(0, 0, 555), red));
	world.add(make_shared<quad>(point3(113, 554, 127), glm::vec3(330, 0, 0), glm::vec3(0, 0, 305), light));
	world.add(make_shared<quad>(point3(0, 555, 0), glm::vec3(555, 0, 0), glm::vec3(0, 0, 555), white));
	world.add(make_shared<quad>(point3(0, 0, 0), glm::vec3(555, 0, 0), glm::vec3(0, 0, 555), white));
	world.add(make_shared<quad>(point3(0, 0, 555), glm::vec3(555, 0, 0), glm::vec3(0, 555, 0), white));

	shared_ptr<hittable> box1 = box(point3(0, 0, 0), point3(165, 330, 165), white);
	box1 = make_shared<rotate_y>(box1, 15);
	box1 = make_shared<translate>(box1, glm::vec3(265, 0, 295));

	shared_ptr<hittable> box2 = box(point3(0, 0, 0), point3(165, 165, 165), white);
	box2 = make_shared<rotate_y>(box2, -18);
	box2 = make_shared<translate>(box2, glm::vec3(130, 0, 65));

	world.add(make_shared<constant_medium>(box1, 0.01f, color(0, 0, 0)));
	world.add(make_shared<constant_medium>(box2, 0.01f, color(1, 1, 1)));

	camera cam;

	cam.aspect_ratio = 1.0;
	cam.image_width = 600;
	cam.samples_per_pixel = 200;
	cam.max_depth = 50;
	cam.background = color(0, 0, 0);
	cam.thread_count = THREAD_COUNT;

	cam.vfov = 40.0f;
	cam.lookfrom = point3(278, 278, -800);
	cam.lookat = point3(278, 278, 0);
	cam.vup = glm::vec3(0, 1, 0);

	cam.defocus_angle = 0;

	cam.render(world);
}

void final_scene(int image_width, int samples_per_pixel, int max_depth) {
	hittable_list boxes1;
	auto ground = make_shared<lambertian>(color(0.48, 0.83, 0.53));

	int boxes_per_side = 20;
	for (int i = 0; i < boxes_per_side; i++) {
		for (int j = 0; j < boxes_per_side; j++) {
			auto w = 100.0;
			auto x0 = -1000.0 + i * w;
			auto z0 = -1000.0 + j * w;
			auto y0 = 0.0;
			auto x1 = x0 + w;
			auto y1 = random_float(1, 101);
			auto z1 = z0 + w;

			boxes1.add(box(point3(x0, y0, z0), point3(x1, y1, z1), ground));
		}
	}

	hittable_list world;

	world.add(make_shared<bvh_node>(boxes1));

	auto light = make_shared<diffuse_light>(color(7, 7, 7));
	world.add(make_shared<quad>(point3(123, 554, 147), glm::vec3(300, 0, 0), glm::vec3(0, 0, 265), light));

	auto center1 = point3(400, 400, 200);
	auto center2 = center1 + glm::vec3(30, 0, 0);
	auto sphere_material = make_shared<lambertian>(color(0.7, 0.3, 0.1));
	world.add(make_shared<sphere>(center1, center2, 50, sphere_material));

	world.add(make_shared<sphere>(point3(260, 150, 45), 50, make_shared<dielectric>(1.5)));
	world.add(make_shared<sphere>(
		point3(0, 150, 145), 50, make_shared<metal>(color(0.8, 0.8, 0.9), 1.0)
	));

	auto boundary = make_shared<sphere>(point3(360, 150, 145), 70, make_shared<dielectric>(1.5));
	world.add(boundary);
	world.add(make_shared<constant_medium>(boundary, 0.2, color(0.2, 0.4, 0.9)));
	boundary = make_shared<sphere>(point3(0, 0, 0), 5000, make_shared<dielectric>(1.5));
	world.add(make_shared<constant_medium>(boundary, .0001, color(1, 1, 1)));

	auto emat = make_shared<lambertian>(make_shared<image_texture>("earthmap.jpg"));
	world.add(make_shared<sphere>(point3(400, 200, 400), 100, emat));
	auto pertext = make_shared<noise_texture>(0.02f, 0.01f);
	world.add(make_shared<sphere>(point3(220, 280, 300), 80, make_shared<lambertian>(pertext)));

	hittable_list boxes2;
	auto white = make_shared<lambertian>(color(.73, .73, .73));
	int ns = 1000;
	for (int j = 0; j < ns; j++) {
		boxes2.add(make_shared<sphere>(point3(random_float(0, 165), random_float(0, 165), random_float(0, 165)), 10, white));
	}

	world.add(make_shared<translate> (
		make_shared<rotate_y>(make_shared<bvh_node>(boxes2), 15),
		glm::vec3(-100, 270, 395)
	)
	);

	camera cam;

	cam.aspect_ratio = 1.0;
	cam.image_width = image_width;
	cam.samples_per_pixel = samples_per_pixel;
	cam.max_depth = max_depth;
	cam.background = color(0, 0, 0);

	cam.vfov = 40.0f;
	cam.lookfrom = point3(478, 278, -600);
	cam.lookat = point3(278, 278, 0);
	cam.vup = glm::vec3(0, 1, 0);

	cam.defocus_angle = 0;

	cam.render(world);
}

int main()
{
	switch (6)
	{
		case 1: bouncing_spheres(); break; 
		case 2: checkered_spheres(); break;
		case 3: earth(); break;
		case 4: perlin_spheres(); break;
		case 5: quads(); break;
		case 6: tris(); break;
		case 7: simple_light(); break;
		case 8: cornell_box(); break;
		case 9: cornell_smoke(); break;
		case 10: final_scene(800, 10000, 50); break;
		default: final_scene(200, 200, 50); break;
	}
	
}