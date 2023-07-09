#include <fstream>
#include <iomanip>
#include <iostream>

#include "Camera.h"
#include "Color.h"
#include "Common.h"
#include "GeometryList.h"
#include "Ray.h"
#include "Sphere.h"

Color ray_color(const Ray& r, const Geometry& world) {
    HitRecord rec;
    if (world.hit(r, 0, INF, rec)) {
        return 0.5 * (rec.normal + Color{1, 1, 1});
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

    // Camera
    Camera cam;

    std::ofstream outfile("test.ppm");

    // World
    GeometryList world;
    world.add(make_shared<Sphere>(Point3d{0, 0, -1}, 0.5));
    world.add(make_shared<Sphere>(Point3d{0, -100.5, -1}, 100));

    // PPM file header
    outfile << "P3" << std::endl
            << image_width << ' ' << image_height << std::endl
            << 255 << std::endl;

    for (int j = image_height - 1; j >= 0; --j) {
        std::cout << "Rendering progress: " << std::fixed
                  << std::setprecision(2)
                  << (double)(image_height - j) / (image_height)*100 << '%'
                  << std::endl;
        for (int i = 0; i < image_width; ++i) {
            Color pixel_color{0, 0, 0};
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                Ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world);
            }
            write_color(outfile, pixel_color, samples_per_pixel);
        }
    }
    std::cout << "Done" << std::endl;
    return 0;
}
