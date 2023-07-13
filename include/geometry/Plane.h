#pragma once

#include <array>

#include "Geometry.h"

class Plane : public Geometry {
   private:
    Point3d _center;
    Vec3d _normal;

   public:
    Plane(Point3d center, Vec3d normal, shared_ptr<Material> material)
        : Geometry(material), _center(center), _normal(normal) {}
    bool hit(const Ray& ray, double t_min, double t_max,
             HitRecord& r_rec) const override {
        double denom = _normal.dot(ray.direction());
        if (std::abs(denom) < std::numeric_limits<double>::epsilon()) {
            // Ray is parallel to the plane
            return false;
        }

        Vec3d oc = _center - ray.origin();
        double t = oc.dot(_normal) / denom;

        if (t < t_min || t > t_max) {
            // Intersection is outside the valid range
            return false;
        }

        r_rec.t = t;
        r_rec.point = ray.at(t);
        r_rec.set_face_normal(ray, _normal);
        r_rec.material = _material;

        return true;
    }
};

class Rectangle : public Geometry {
   private:
    std::array<Point3d, 4> _vertices;
    Vec3d _normal;

   public:
    Rectangle(std::array<Point3d, 4> vertices, const Vec3d& normal,
              shared_ptr<Material> material)
        : Geometry(material), _normal(normal) {
        _vertices[0] = vertices[0];
        _vertices[1] = vertices[1];
        _vertices[2] = vertices[2];
        _vertices[3] = vertices[3];
    }

    bool hit(const Ray& ray, double t_min, double t_max,
             HitRecord& r_rec) const override {
        Vec3d op = _vertices[0] - ray.origin();
        double denom = ray.direction().dot(_normal);

        if (std::abs(denom) < std::numeric_limits<double>::epsilon()) {
            // Ray is parallel to the plane
            return false;
        }

        double t = op.dot(_normal) / denom;

        if (t < t_min || t > t_max) {
            return false;
        }

        Vec3d hitPoint = ray.at(t);
        if (!insideRectangle(hitPoint)) {
            return false;
        }

        r_rec.t = t;
        r_rec.point = hitPoint;
        r_rec.set_face_normal(ray, _normal);
        r_rec.material = _material;
        return true;
    }

   private:
    bool insideRectangle(const Point3d& point) const {
        Vec3d edges[4] = {
            _vertices[1] - _vertices[0], _vertices[2] - _vertices[1],
            _vertices[3] - _vertices[2], _vertices[0] - _vertices[3]};

        Vec3d vectors[4] = {point - _vertices[0], point - _vertices[1],
                            point - _vertices[2], point - _vertices[3]};

        for (int i = 0; i < 4; ++i) {
            Vec3d cross = edges[i].cross(vectors[i]);
            if (cross.dot(_normal) >= 0) {
                return false;
            }
        }

        return true;
    }
};