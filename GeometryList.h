#pragma once

#include <memory>
#include <vector>

#include "Geometry.h"

using std::shared_ptr;

class GeometryList : public Geometry {
   public:
    GeometryList() {}
    GeometryList(shared_ptr<Geometry> object) { add(object); }

    void clear() { geometries.clear(); }
    void add(shared_ptr<Geometry> object) { geometries.push_back(object); }

    virtual bool hit(const Ray& r, double t_min, double t_max,
                     HitRecord& rec) const override {
        HitRecord temp_rec;
        bool hit_anything = false;
        auto closest_so_far = t_max;

        for (const auto& geometry : geometries) {
            if (geometry->hit(r, t_min, closest_so_far, temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }
        return hit_anything;
    }

   public:
    std::vector<shared_ptr<Geometry>> geometries;
};