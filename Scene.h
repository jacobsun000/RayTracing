#pragma once

#include "Camera.h"
#include "Common.h"
#include "GeometryList.h"
#include "RenderingOption.h"
#include "Sphere.h"

class Scene {
   public:
    Scene(Camera camera, GeometryList objects, RenderingOption option)
        : camera(camera), objects(objects), option(option) {}

    static GeometryList random() {
    GeometryList world;

    auto ground_material = make_shared<Lambertian>(Color{0.5, 0.5, 0.5});
    world.add(make_shared<Sphere>(Point3d{0, -1000, 0}, 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = Math::random_double();
            Point3d center{a + 0.9 * Math::random_double(), 0.2,
                           b + 0.9 * Math::random_double()};

            if ((center - Point3d{4, 0.2, 0}).length() > 0.9) {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = Color::random() * Color::random();
                    sphere_material = make_shared<Lambertian>(albedo);
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = Color::random(0.5, 1);
                    auto fuzz = Math::random_double(0, 0.5);
                    sphere_material = make_shared<Metal>(albedo, fuzz);
                } else {
                    // glass
                    sphere_material = make_shared<Dielectric>(1.5);
                }

                world.add(make_shared<Sphere>(center, 0.2, sphere_material));
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    world.add(make_shared<Sphere>(Point3d{0, 1, 0}, 1.0, material1));

    auto material2 = make_shared<Lambertian>(Color{0.4, 0.2, 0.1});
    world.add(make_shared<Sphere>(Point3d{-4, 1, 0}, 1.0, material2));

    auto material3 = make_shared<Metal>(Color{0.7, 0.6, 0.5}, 0.0);
    world.add(make_shared<Sphere>(Point3d{4, 1, 0}, 1.0, material3));

    return world;
}

   public:
    Camera camera;
    GeometryList objects;
    RenderingOption option;
};