#include <fstream>
#include <iomanip>
#include <iostream>

#include "Camera.h"
#include "Image.h"
#include "Renderer.h"
#include "Scene.h"
#include "SceneBuilder.h"

using namespace Math;

int main(int argc, char const *argv[]) {
    double aspect_ratio = 16.0 / 9.0;
    int width = 200;
    int height = static_cast<int>(width / aspect_ratio);
    int samples_per_pixel = 100;
    int max_depth = 50;
    std::string outfile = "test.ppm";

    ImageOption imageOption{width, height};
    RenderOption renderOption{samples_per_pixel, max_depth};
    GeometryList objects = SceneBuilder::random();

    Point3d lookfrom{13, 2, 3};
    Point3d lookat{0, 0, 0};
    Vec3d vup{0, 1, 0};
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    Camera camera(lookfrom, lookat, vup, 20, aspect_ratio, aperture,
                  dist_to_focus);

    Scene scene(camera, objects);
    PPM_Image image(imageOption, outfile);
    Renderer renderer(scene);
    renderer.render(renderOption, image);
    image.write();
    return 0;
}
