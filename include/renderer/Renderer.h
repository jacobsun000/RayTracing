#pragma once

#include <atomic>
#include <thread>

#include "Color.h"
#include "Common.h"
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
   protected:
    Scene _scene;

   public:
    Renderer(Scene scene) : _scene(scene) {}
    virtual ~Renderer() = default;
    virtual void render(RenderOption option, Image& output) = 0;
};

using RendererPtr = shared_ptr<Renderer>;

class CPU_ST_Renderer : public Renderer {
   public:
    CPU_ST_Renderer(Scene scene) : Renderer(scene) {}
    void render(RenderOption option, Image& output) override {
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
                Pixel color =
                    color_to_rgb<ComponentType>(pixel_color, samples_per_pixel);
                output.data[height - y - 1][x] = color;
            }
        }
    }
};

class CPU_MT_Renderer : public Renderer {
   public:
    CPU_MT_Renderer(Scene scene) : Renderer(scene) {}

    void render(RenderOption option, Image& output) override {
        int width = output.width;
        int height = output.height;
        int samples_per_pixel = option.samples_per_pixel;
        int max_depth = option.max_depth;
        unsigned int num_threads = std::thread::hardware_concurrency();

        vector<std::thread> threads(num_threads);
        std::atomic<int> completed_rows{0};

        int rows_per_thread = height / num_threads;
        int extra_rows = height % num_threads;
        int start_y = 0;
        int end_y = rows_per_thread;

        for (unsigned int thread_id = 0; thread_id < num_threads; ++thread_id) {
            if (extra_rows > 0) {
                end_y++;
                extra_rows--;
            }
            threads[thread_id] = std::thread([this, &output, &completed_rows,
                                              thread_id, start_y, end_y, width,
                                              height, samples_per_pixel,
                                              max_depth]() {
                for (int y = start_y; y < end_y; ++y) {
                    ++completed_rows;
                    for (int x = 0; x < width; ++x) {
                        Color pixel_color{0, 0, 0};
                        for (int s = 0; s < samples_per_pixel; ++s) {
                            auto u = (x + Math::random_double()) / (width - 1);
                            auto v = (y + Math::random_double()) / (height - 1);
                            Ray r = _scene.camera.get_ray(u, v);
                            pixel_color +=
                                ray_color(r, _scene.objects, max_depth);
                        }
                        Pixel color = color_to_rgb<ComponentType>(
                            pixel_color, samples_per_pixel);
                        output.data[height - y - 1][x] = color;
                    }
                }
            });
            start_y = end_y;
            end_y += rows_per_thread;
        }

        while (completed_rows < height - 1) {
            showProgressBar(static_cast<double>(completed_rows) / (height - 1));
            std::this_thread::sleep_for(std::chrono::duration<double>(0.5));
        }
        std::cout << std::endl;

        for (auto& thread : threads) {
            thread.join();
        }
    }
};