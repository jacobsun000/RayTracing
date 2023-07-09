#pragma once

#include <cmath>

#include "Ray.h"
#include "Vector.h"

struct HitRecord {
    Point3d point;
    Vec3d normal;
    double t;
    bool front_face;
    inline void set_face_normal(const Ray& r, const Vec3d& outward_normal) {
        front_face = r.direction() * outward_normal < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Geometry {
   public:
    virtual ~Geometry() = default;
    // Returns normal
    virtual bool hit(const Ray& ray, double t_min, double t_max,
                     HitRecord& r_rec) const = 0;
};