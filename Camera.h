#pragma once
#include "Common.h"

class Camera {
   public:
    Camera(Point3d lookfrom, Point3d lookat, Vec3d vup, double vfov,
           double aspect_ratio, double aperture, double focus_dist) {
        auto theta = Math::deg_to_rad(vfov);
        auto h = tan(theta / 2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        w = (lookfrom - lookat).unit_vector();
        u = (vup.cross(w)).unit_vector();
        v = w.cross(u);

        _origin = lookfrom;
        _horizontal = focus_dist * viewport_width * u;
        _vertical = focus_dist * viewport_height * v;
        _lower_left_corner =
            _origin - _horizontal / 2 - _vertical / 2 - focus_dist * w;

        _lens_radius = aperture / 2;
    }

    Ray get_ray(double s, double t) const {
        Vec3d rd = _lens_radius * Math::random_in_unit_disk();
        Vec3d offset = u * rd.x() + v * rd.y();

        return Ray(_origin + offset, _lower_left_corner + s * _horizontal +
                                         t * _vertical - _origin - offset);
    }

   private:
    Point3d _origin;
    Point3d _lower_left_corner;
    Vec3d _horizontal;
    Vec3d _vertical;
    double _lens_radius;
    Vec3d u, v, w;
};