#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "texture.h"

class material 
{
public:
	virtual ~material() = default;

	virtual bool scatter(
		const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
	) const 
	{
		return false;
	}
};

class lambertian : public material 
{
public:
	lambertian(const color& albedo) : tex(make_shared<solid_color>(albedo)) {}
	lambertian(shared_ptr<texture> tex) : tex(tex) {}

	bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
		const override 
	{
		auto scatter_direction = rec.normal + random_unit_vector();

		if (near_zero(scatter_direction))
			scatter_direction = rec.normal;

		scattered = ray(rec.p, scatter_direction, r_in.time());
		attenuation = tex->value(rec.u,rec.v,rec.p);
		return true;
	}

private:
	shared_ptr<texture> tex;
};

class metal : public material {
  public:
	  metal(const color& albedo, float fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
    const override
	{
        glm::vec3 reflected = reflect(r_in.direction(), rec.normal);
		reflected = glm::normalize(reflected) + (fuzz * random_unit_vector());
        scattered = ray(rec.p, reflected, r_in.time());
        attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
    }

  private:
    color albedo;
	float fuzz;
};

class dielectric : public material 
{
  public:
	dielectric(float refraction_index) : refraction_index(refraction_index) {}

	bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
	const override 
	{
		attenuation = color(1.0f, 1.0f, 1.0f);
		float ri = rec.front_face ? (1.0f /refraction_index) : refraction_index;

		glm::vec3 unit_direction = glm::normalize(r_in.direction());
		float cos_theta = std::fminf(dot(-unit_direction, rec.normal), 1.0f);
		float sin_theta = std::sqrtf(1.0f - cos_theta * cos_theta);

		bool cannot_refract = ri * sin_theta > 1.0f;
		glm::vec3 direction;

		if (cannot_refract || reflectance(cos_theta, ri) > random_float())
			direction = reflect(unit_direction, rec.normal);
		else
			direction = refract(unit_direction, rec.normal, ri);

		scattered = ray(rec.p, direction, r_in.time());
		return true;
	}
  private:
	float refraction_index;

	static float reflectance(float cosine, float ref_idx) 
	{
		// Use Schlick's approximation for reflectance.
		auto r0 = (1.0f - ref_idx) / (1.0f + ref_idx);
		r0 = r0 * r0;
		return r0 + (1.0f - r0) * std::powf((1.0f - cosine), 5.0f);
	}
};

#endif
