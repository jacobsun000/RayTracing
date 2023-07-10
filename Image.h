#pragma once

#include <fstream>
#include <iostream>
#include <string>

#include "Color.h"
#include "ProgressBar.h"
#include "Renderer.h"
#include "Scene.h"

class Image {
   public:
    Image(Scene scene) : _scene(scene) {}
    virtual void write() = 0;

   protected:
    Scene _scene;
};

class PPM_Image : public Image {
   public:
    PPM_Image(Scene scene, const std::string& filename)
        : Image(scene), _filename(filename) {}
    void write() override {
        int width = _scene.option.image_width;
        int height = _scene.option.image_height;
        int max_depth = _scene.option.max_depth;
        int samples_per_pixel = _scene.option.samples_per_pixel;
        std::ofstream outfile(_filename);

        outfile << "P3\n"
                << _scene.option.image_width << ' '
                << _scene.option.image_height << '\n'
                << 255 << '\n';

        for (int j = height - 1; j >= 0; --j) {
            showProgressBar((double)(height - j) / height);
            for (int i = 0; i < width; ++i) {
                Color pixel_color{0, 0, 0};
                for (int s = 0; s < samples_per_pixel; ++s) {
                    auto u = (i + Math::random_double()) / (width - 1);
                    auto v = (j + Math::random_double()) / (height - 1);
                    Ray r = _scene.camera.get_ray(u, v);
                    pixel_color += ray_color(r, _scene.objects, max_depth);
                }
                Vec<int, 3> pixel =
                    color_to_rgb(pixel_color, samples_per_pixel);
                outfile << pixel.x() << ' ' << pixel.y() << ' ' << pixel.z()
                        << '\n';
            }
        }
    }

   private:
    std::string _filename;
};