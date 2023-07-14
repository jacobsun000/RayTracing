#pragma once

#include "Color.h"
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