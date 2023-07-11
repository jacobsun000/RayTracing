#pragma once

#include "Geometry.h"

class Sphere : public Geometry {
   public:
    Sphere(Point3d center, double radius, shared_ptr<Material> material)
        : Geometry(material), _center(center), _radius(radius) {}
    bool hit(const Ray& ray, double t_min, double t_max,
             HitRecord& r_rec) const override {
        Vec3d oc = ray.origin() - _center;
        double a = ray.direction().length_squared();
        double half_b = oc.dot(ray.direction());
        double c = oc.length_squared() - _radius * _radius;

        double discriminant = half_b * half_b - a * c;
        if (discriminant < 0) return false;
        double sqrtd = sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        double root = (-half_b - sqrtd) / a;
        if (root < t_min || t_max < root) {
            root = (-half_b + sqrtd) / a;
            if (root < t_min || t_max < root) return false;
        }

        r_rec.t = root;
        r_rec.point = ray.at(r_rec.t);
        Vec3d outward_normal = (r_rec.point - _center) / _radius;
        r_rec.set_face_normal(ray, outward_normal);
        r_rec.material = _material;

        return true;
    }

   private:
    Point3d _center;
    double _radius;
};