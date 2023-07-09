#pragma once

#include "Vector.h"

class Ray {
   public:
    Ray() {}
    Ray(const Point3d& origin, const Point3d& direction)
        : _origin(origin), _direction(direction) {}

    Point3d origin() const { return _origin; }
    Vec3d direction() const { return _direction; }

    Point3d at(double t) const { return _origin + t * _direction; }

   private:
    Point3d _origin;
    Vec3d _direction;
};