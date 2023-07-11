#pragma once

#include <vector>

#include "Geometry.h"

class GeometryList : public Geometry {
   public:
    GeometryList() : Geometry(nullptr) {}
    GeometryList(shared_ptr<Geometry> object) : Geometry(nullptr) {
        add(object);
    }

    void clear() { _geometries.clear(); }
    void add(shared_ptr<Geometry> object) { _geometries.push_back(object); }

    virtual bool hit(const Ray& r, double t_min, double t_max,
                     HitRecord& rec) const override {
        HitRecord temp_rec;
        bool hit_anything = false;
        auto closest_so_far = t_max;

        for (const auto& geometry : _geometries) {
            if (geometry->hit(r, t_min, closest_so_far, temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }
        return hit_anything;
    }

   private:
    std::vector<shared_ptr<Geometry>> _geometries;
};