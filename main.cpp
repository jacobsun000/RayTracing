#include <fstream>
#include <iomanip>
#include <iostream>

#include "Camera.h"
#include "Common.h"
#include "Image.h"
#include "RenderingOption.h"
#include "Scene.h"

using namespace Math;

int main(int argc, char const *argv[]) {
    double aspect_ratio = 3.0 / 2.0;
    RenderingOption option(aspect_ratio, 400,
                           static_cast<int>(400 / aspect_ratio), 100, 50);
    GeometryList objects = Scene::random();

    Point3d lookfrom{13, 2, 3};
    Point3d lookat{0, 0, 0};
    Vec3d vup{0, 1, 0};
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    Camera camera(lookfrom, lookat, vup, 20, aspect_ratio, aperture,
                  dist_to_focus);

    Scene scene(camera, objects, option);
    PPM_Image image(scene, "test.ppm");
    image.write();
    return 0;
}
