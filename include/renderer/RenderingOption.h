#pragma once

struct RenderingOption {
    double aspect_ratio;
    int image_width;
    int image_height;
    int samples_per_pixel;
    int max_depth;

    RenderingOption(double aspect_ratio, int image_width, int image_height,
                    int samples_per_pixel, int max_depth)
        : aspect_ratio(aspect_ratio),
          image_width(image_width),
          image_height(image_height),
          samples_per_pixel(samples_per_pixel),
          max_depth(max_depth) {}
};