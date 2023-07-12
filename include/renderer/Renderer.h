#pragma once

#include "Color.h"
#include "Geometry.h"
#include "Image.h"
#include "Material.h"
#include "ProgressBar.h"
#include "Scene.h"

Color ray_color(const Ray& r, const Geometry& world, int depth) {
    if (depth <= 0) return Color{0, 0, 0};
    HitRecord rec;
    if (world.hit(r, 0.001, Math::INF, rec)) {
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

struct RenderOption {
    int samples_per_pixel;
    int max_depth;
};

class Renderer {
   private:
    Scene _scene;

   public:
    Renderer(Scene scene) : _scene(scene) {}
    void render(RenderOption option, Image& output) {
        int width = output.width;
        int height = output.height;
        int samples_per_pixel = option.samples_per_pixel;
        int max_depth = option.max_depth;

        for (int y = 0; y < height; ++y) {
            showProgressBar(static_cast<double>(y) / height);
            for (int x = 0; x < width; ++x) {
                Color pixel_color{0, 0, 0};
                for (int s = 0; s < samples_per_pixel; ++s) {
                    auto u = (x + Math::random_double()) / (width - 1);
                    auto v = (y + Math::random_double()) / (height - 1);
                    Ray r = _scene.camera.get_ray(u, v);
                    pixel_color += ray_color(r, _scene.objects, max_depth);
                }
                Pixel color = color_to_rgb<int>(pixel_color, samples_per_pixel);
                output.data[height - y - 1][x] = color;
            }
        }
    }
};