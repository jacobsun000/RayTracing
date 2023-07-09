#pragma once

#include "Ray.h"
#include "Vector.h"

class Geometry {
   public:
    virtual ~Geometry() = default;
    virtual bool hit(const Ray& ray) const = 0;
};

class Sphere : Geometry {
   public:
    Sphere(Point3d center, double radius) : _center(center), _radius(radius) {}
    bool hit(const Ray& ray) const override {
        Vec3d oc = ray.origin() - _center;
        Vec3d a = ray.direction() * ray.direction();
        Vec3d b = 2.0 * oc * ray.direction();
        Vec3d c = oc * oc - _radius * _radius;
        double discriminant = b * b - 4 * a * c;
        return discriminant > 0;
    }

   private:
    Point3d _center;
    double _radius;
};