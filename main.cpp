#include <fstream>
#include <iomanip>
#include <iostream>

#include "Color.h"
#include "Ray.h"
#include "Vector.h"

Color ray_color(const Ray ray) {
    Vec3d unit_direction = ray.direction().unit_vector();
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * Color{1.0, 1.0, 1.0} + t *Color{0.5, 0.7, 1.0};
}

int main(int argc, char const *argv[]) {
    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // Camera

    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    Point3d origin{0, 0, 0};
    Vec3d horizontal{viewport_width, 0, 0};
    Vec3d vertical{0, viewport_height, 0};
    Vec3d lower_left_corner =
        origin - horizontal / 2 - vertical / 2 - Vec3d{0, 0, focal_length};

    std::ofstream outfile("test.ppm");
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
            auto u = double(i) / (image_width - 1);
            auto v = double(j) / (image_height - 1);
            Ray r(origin,
                  lower_left_corner + u * horizontal + v * vertical - origin);
            Color pixel_color = ray_color(r);
            write_color(outfile, pixel_color);
        }
    }
    std::cout << "Done" << std::endl;
    return 0;
}
