#pragma once

#include "Color.h"
#include "Common.h"
#include "Geometry.h"

Vec3d reflect(const Vec3d& v, const Vec3d& n) { return v - 2 * v.dot(n) * n; }

Vec3d refract(const Vec3d& uv, const Vec3d& n, double etai_over_etat) {
    auto cos_theta = fmin((-uv).dot(n), 1.0);
    Vec3d r_out_perp = etai_over_etat * (uv + cos_theta * n);
    Vec3d r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

class Material {
   public:
    virtual ~Material() = default;
    virtual bool scatter(const Ray& ray, const HitRecord& rec,
                         Color& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material {
   public:
    Lambertian(const Color& albedo) : _albedo(albedo) {}

    bool scatter(const Ray& ray, const HitRecord& rec, Color& attenuation,
                 Ray& scattered) const override {
        auto scatter_direction = rec.normal + Math::random_unit_vector();
        // Catch degenerate scatter direction
        if (scatter_direction.near_zero()) scatter_direction = rec.normal;
        scattered = Ray(rec.point, scatter_direction);
        attenuation = _albedo;
        return true;
    }

   private:
    Color _albedo;
};

class Metal : public Material {
   public:
    Metal(const Color& albedo, double fuzz)
        : _albedo(albedo), _fuzz(fuzz < 1 ? fuzz : 1) {}

    bool scatter(const Ray& ray, const HitRecord& rec, Color& attenuation,
                 Ray& scattered) const override {
        Vec3d reflected = reflect(ray.direction().unit_vector(), rec.normal);
        scattered =
            Ray(rec.point, reflected + _fuzz * Math::random_in_unit_sphere());
        attenuation = _albedo;
        return scattered.direction().dot(rec.normal) > 0;
    }

   private:
    Color _albedo;
    double _fuzz;
};

class Dielectric : public Material {
   public:
    Dielectric(double refraction_rate) : _refraction_rate(refraction_rate) {}

    virtual bool scatter(const Ray& ray, const HitRecord& rec,
                         Color& attenuation, Ray& scattered) const override {
        attenuation = Color{1.0, 1.0, 1.0};
        double refraction_ratio =
            rec.front_face ? (1.0 / _refraction_rate) : _refraction_rate;
        Vec3d unit_direction = ray.direction().unit_vector();
        double cos_theta = fmin((-unit_direction).dot(rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        Vec3d direction;

        if (cannot_refract ||
            reflectance(cos_theta, refraction_ratio) > Math::random_double())
            direction = reflect(unit_direction, rec.normal);
        else
            direction = refract(unit_direction, rec.normal, refraction_ratio);

        scattered = Ray(rec.point, direction);
        return true;
    }

   private:
    double _refraction_rate;  // Index of Refraction

    static double reflectance(double cosine, double ref_idx) {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
};