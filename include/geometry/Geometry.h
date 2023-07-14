#pragma once

#include <cmath>

#include "Ray.h"
#include "Vector.h"

class Material;

struct HitRecord {
    Point3d point;
    Vec3d normal;
    double t;
    bool front_face;
    shared_ptr<Material> material;
    inline void set_face_normal(const Ray& r, const Vec3d& outward_normal) {
        front_face = r.direction().dot(outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Geometry {
   protected:
    shared_ptr<Material> _material;

   public:
    Geometry(shared_ptr<Material> material) : _material(material) {}
    virtual ~Geometry() = default;
    // Returns normal
    virtual bool hit(const Ray& ray, double t_min, double t_max,
                     HitRecord& r_rec) const = 0;
};