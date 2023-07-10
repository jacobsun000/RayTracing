#pragma once
#include "Common.h"

class Camera {
   public:
    Camera(Point3d lookfrom, Point3d lookat, Vec3d vup,
           double vfov,  // vertical field-of-view in degrees
           double aspect_ratio) {
        auto theta = Math::deg_to_rad(vfov);
        auto h = tan(theta / 2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        auto w = (lookfrom - lookat).unit_vector();
        auto u = (vup.cross(w)).unit_vector();
        ;
        auto v = w.cross(u);

        _origin = lookfrom;
        _horizontal = viewport_width * u;
        _vertical = viewport_height * v;
        _lower_left_corner = _origin - _horizontal / 2 - _vertical / 2 - w;
    }

    Ray get_ray(double s, double t) const {
        return Ray(_origin, _lower_left_corner + s * _horizontal +
                                t * _vertical - _origin);
    }

   private:
    Point3d _origin;
    Point3d _lower_left_corner;
    Vec3d _horizontal;
    Vec3d _vertical;
};