#pragma once
#include "Common.h"

class Camera {
   public:
    Camera() {
        auto aspect_ratio = 16.0 / 9.0;
        auto viewport_height = 2.0;
        auto viewport_width = aspect_ratio * viewport_height;
        auto focal_length = 1.0;

        _origin = Point3d{0, 0, 0};
        _horizontal = Vec3d{viewport_width, 0.0, 0.0};
        _vertical = Vec3d{0.0, viewport_height, 0.0};
        _lower_left_corner = _origin - _horizontal / 2 - _vertical / 2 -
                             Vec3d{0, 0, focal_length};
    }

    Ray get_ray(double u, double v) const {
        return Ray(_origin, _lower_left_corner + u * _horizontal +
                                v * _vertical - _origin);
    }

   private:
    Point3d _origin;
    Point3d _lower_left_corner;
    Vec3d _horizontal;
    Vec3d _vertical;
};