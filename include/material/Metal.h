#include "Material.h"

class Metal : public Material {
   private:
    Color _albedo;
    double _fuzz;

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
};