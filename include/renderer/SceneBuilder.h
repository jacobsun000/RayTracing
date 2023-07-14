#pragma once

#include "Dielectric.h"
#include "Lambertian.h"
#include "Metal.h"
#include "Scene.h"

class SceneBuilder {
    using P = Point3d;
    using V = Vec3d;
    using C = Color;

   private:
    template <typename T, typename... Args>
    static std::shared_ptr<T> ptr(Args&&... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

   public:
    static Scene cornel_box() {
        GeometryList world;
        // Walls
        auto m_white_wall = ptr<Metal>(C{0.9, 0.9, 0.9}, 0.96);
        auto m_red_wall = ptr<Lambertian>(C{1, 0.01, 0.01});
        auto m_green_wall = ptr<Lambertian>(C{0.01, 1, 0.01});
        P box_points[8] = {{-5, -5, -5}, {5, -5, -5}, {5, 5, -5}, {-5, 5, -5},
                           {-5, -5, 5},  {5, -5, 5},  {5, 5, 5},  {-5, 5, 5}};
        auto make_wall = [&](std::array<int, 4> pts, V n, auto m) {
            world.add(ptr<Rectangle>(
                std::array<P, 4>{box_points[pts[0]], box_points[pts[1]],
                                 box_points[pts[2]], box_points[pts[3]]},
                n, m));
        };
        make_wall({0, 1, 5, 4}, {0, 1, 0}, m_red_wall);     // left
        make_wall({2, 3, 7, 6}, {0, -1, 0}, m_green_wall);  // right
        make_wall({0, 4, 7, 3}, {1, 0, 0}, m_white_wall);   // front
        make_wall({0, 3, 2, 1}, {0, 0, 1}, m_white_wall);   // bottom
        make_wall({4, 5, 6, 7}, {0, 0, -1}, m_white_wall);  // top

        // Balls
        auto make_ball = [&](P c, double r, shared_ptr<Material> m) {
            world.add(ptr<Sphere>(c, r, m));
        };
        auto m_glass = make_shared<Dielectric>(0.9);
        auto m_smooth = make_shared<Lambertian>(C{0.4, 0.2, 0.1});
        auto m_metal = make_shared<Metal>(C{0.7, 0.6, 0.5}, 0.0);
        make_ball(P{-3, -2, -3.5}, 1.5, m_smooth);  // left
        make_ball(P{-3, 2, -3.5}, 1.5, m_metal);    // right
        make_ball(P{0, 0, -3.5}, 1.5, m_glass);     // mid
        //  Camera
        Point3d lookfrom{10, 0, 1};
        Point3d lookat{0, 0, 0};
        Vec3d vup{0, 0, 1};
        auto dist_to_focus = 8.0;
        auto aperture = 0.01;
        double aspect_ratio = 16.0 / 9.0;
        Camera camera(lookfrom, lookat, vup, 50, aspect_ratio, aperture,
                      dist_to_focus);

        Scene scene(camera, world);
        return scene;
    }

    static Scene random_spheres() {
        GeometryList world;

        auto ground_material = make_shared<Lambertian>(Color{0.5, 0.5, 0.5});
        world.add(make_shared<Plane>(Point3d{0, 0, 0}, Point3d{0, 1, 0},
                                     ground_material));

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

                    world.add(
                        make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }

        auto material1 = make_shared<Dielectric>(1.5);
        world.add(make_shared<Sphere>(Point3d{0, 1, 0}, 1.0, material1));

        auto material2 = make_shared<Lambertian>(Color{0.4, 0.2, 0.1});
        world.add(make_shared<Sphere>(Point3d{-4, 1, 0}, 1.0, material2));

        auto material3 = make_shared<Metal>(Color{0.7, 0.6, 0.5}, 0.0);
        world.add(make_shared<Sphere>(Point3d{4, 1, 0}, 1.0, material3));

        // Camera
        Point3d lookfrom{13, 2, 3};
        Point3d lookat{0, 0, 0};
        Vec3d vup{0, 1, 0};
        auto dist_to_focus = 10.0;
        auto aperture = 0.1;
        double aspect_ratio = 16.0 / 9.0;
        Camera camera(lookfrom, lookat, vup, 20, aspect_ratio, aperture,
                      dist_to_focus);

        Scene scene(camera, world);
        return scene;
    }
};