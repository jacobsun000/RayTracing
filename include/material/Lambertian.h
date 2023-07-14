#include "Material.h"

class Lambertian : public Material {
   private:
    Color _albedo;

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
};