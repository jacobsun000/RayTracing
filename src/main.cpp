#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "Camera.h"
#include "Image.h"
#include "Renderer.h"
#include "Scene.h"
#include "SceneBuilder.h"

int main(int argc, char const *argv[]) {
    double aspect_ratio = 16.0 / 9.0;
    int width = 1920;
    int height = static_cast<int>(width / aspect_ratio);
    int samples_per_pixel = 400;
    int max_depth = 100;
    std::string outfile = "test.ppm";

    ImageOption imageOption{width, height};
    RenderOption renderOption{samples_per_pixel, max_depth};
    Scene scene = SceneBuilder::cornel_box();

    PPM_Image image(imageOption, outfile);
    RendererPtr renderer = make_shared<CPU_MT_Renderer>(scene);

    auto time = []() { return std::chrono::steady_clock::now(); };
    auto start_time = time();
    renderer->render(renderOption, image);
    std::cout << "Rendering time: "
              << std::chrono::duration_cast<std::chrono::seconds>(time() -
                                                                  start_time)
                     .count()
              << "s" << std::endl;
    start_time = time();
    image.write();
    std::cout << "Image output time: "
              << std::chrono::duration_cast<std::chrono::seconds>(time() -
                                                                  start_time)
                     .count()
              << "s" << std::endl;

    return 0;
}
