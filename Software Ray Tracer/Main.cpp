#include "rtweekend_commons.h"

#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

color ray_color(const ray& r, const hittable& world) {
	hit_record rec;
	if (world.hit(r, interval(0, infinity), rec)) {
		return 0.5f * (rec.normal + color(1, 1, 1));
	}
		

	auto unit_direction = glm::normalize(r.direction());
	auto a = 0.5f * (unit_direction.y + 1.0f);
	return (1.0f - a) * color(1.0f, 1.0f, 1.0f) + a * color(0.5f, 0.7f, 1.0f);
}

int main()
{
	auto aspect_ratio = 16.0 / 9.0;
	int image_width = 400;

	// Calculate the image height, and ensure that it's at least 1.
	int image_height = int(image_width / aspect_ratio);
	image_height = (image_height < 1) ? 1 : image_height;

	// World
	hittable_list world;
	world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
	world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));	

	// Camera
	auto focal_length = 1.0; // The distance from the camera center to the projection plane.
	auto viewport_height = 2.0;
	auto viewport_width = viewport_height * (double(image_width) / image_height);
	auto camera_center = point3(0, 0, 0);

	// Calculate the vectors across the horizontal and down the vertical viewport edges.
	auto viewport_u = glm::vec3(viewport_width, 0, 0);
	auto viewport_v = glm::vec3(0, -viewport_height, 0);

	// Calculate the horizontal and vertical delta vectors from pixel to pixel.
	auto pixel_delta_u = viewport_u / (float)image_width;
	auto pixel_delta_v = viewport_v / (float)image_height;

	// Calculate the location of the upper left pixel.
	auto viewport_upper_left = camera_center
		- glm::vec3(0, 0, focal_length) - viewport_u / 2.0f - viewport_v / 2.0f;
	// The location of the upper left pixel is half a pixel delta down and to the right 
	// from the upper left corner of the viewport.
	auto pixel00_loc = viewport_upper_left + 0.5f * (pixel_delta_u + pixel_delta_v);


	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for(int j = 0; j < image_height; ++j)
	{
		std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
		for (int i = 0; i < image_width; ++i)
		{
			auto pixel_center = pixel00_loc + ((float)i * pixel_delta_u) + ((float)j * pixel_delta_v);
			auto ray_direction = pixel_center - camera_center;
			ray r(camera_center, ray_direction);

			color pixel_color = ray_color(r, world);
			write_color(std::cout, pixel_color);
		}
	}

	std::clog << "\rDone.                 \n";
}