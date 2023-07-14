#include "Material.h"

class Dielectric : public Material {
   private:
    double _refraction_rate;  // Index of Refraction

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
    static double reflectance(double cosine, double ref_idx) {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
};