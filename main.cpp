#include <fstream>
#include <iomanip>
#include <iostream>

#include "Camera.h"
#include "Color.h"
#include "Common.h"
#include "GeometryList.h"
#include "Material.h"
#include "Math.h"
#include "ProgressBar.h"
#include "Ray.h"
#include "Sphere.h"

using namespace Math;

Color ray_color(const Ray& r, const Geometry& world, int depth) {
    if (depth <= 0) return Color{0, 0, 0};
    HitRecord rec;
    if (world.hit(r, 0.001, INF, rec)) {
        Ray scattered;
        Color attenuation;
        if (rec.material->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth - 1);
        return Color{0, 0, 0};
    }
    Vec3d unit_direction = r.direction().unit_vector();
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * Color{1.0, 1.0, 1.0} + t* Color{0.5, 0.7, 1.0};
}

GeometryList random_scene() {
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
                    auto fuzz = random_double(0, 0.5);
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

int main(int argc, char const *argv[]) {
    // Image
    const auto aspect_ratio = 3.0 / 2.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;
    std::ofstream outfile("test.ppm");

    // World
    GeometryList world = random_scene();

    // Camera
    Point3d lookfrom{13, 2, 3};
    Point3d lookat{0, 0, 0};
    Vec3d vup{0, 1, 0};
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    Camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture,
               dist_to_focus);

    // PPM file header
    outfile << "P3" << std::endl
            << image_width << ' ' << image_height << std::endl
            << 255 << std::endl;

    for (int j = image_height - 1; j >= 0; --j) {
        showProgressBar((double)(image_height - j) / image_height);
        for (int i = 0; i < image_width; ++i) {
            Color pixel_color{0, 0, 0};
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                Ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(outfile, pixel_color, samples_per_pixel);
        }
    }
    return 0;
}
