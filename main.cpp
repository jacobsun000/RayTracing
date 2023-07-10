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

int main(int argc, char const *argv[]) {
    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;
    std::ofstream outfile("test.ppm");

    // World
    GeometryList world;

    auto material_ground = make_shared<Lambertian>(Color{0.8, 0.8, 0.0});
    auto material_center = make_shared<Lambertian>(Color{0.1, 0.2, 0.5});
    auto material_left = make_shared<Dielectric>(1.5);
    auto material_right = make_shared<Metal>(Color{0.8, 0.6, 0.2}, 0.0);

    world.add(make_shared<Sphere>(Point3d{0.0, -100.5, -1.0}, 100.0,
                                  material_ground));
    world.add(
        make_shared<Sphere>(Point3d{0.0, 0.0, -1.0}, 0.5, material_center));
    world.add(
        make_shared<Sphere>(Point3d{-1.0, 0.0, -1.0}, 0.5, material_left));
    world.add(
        make_shared<Sphere>(Point3d{-1.0, 0.0, -1.0}, -0.45, material_left));
    world.add(
        make_shared<Sphere>(Point3d{1.0, 0.0, -1.0}, 0.5, material_right));

    // Camera
    Camera cam(Point3d{-2, 2, 1}, Point3d{0, 0, -1}, Vec3d{0, 1, 0}, 20,
               aspect_ratio);

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
