#pragma once

#include <fstream>
#include <sstream>
#include <string>

#include "MathUtils.h"
#include "Vector.h"

using Color = Vec3d;

Color from_hex(const std::string& hexColor) {
    std::istringstream iss(hexColor.substr(1));
    int hexValue;
    iss >> std::hex >> hexValue;

    double r = ((hexValue >> 16) & 0xFF) / 255.0;
    double g = ((hexValue >> 8) & 0xFF) / 255.0;
    double b = ((hexValue >> 0) & 0xFF) / 255.0;
    return {r, g, b};
}

Vec<int, 3> color_to_rgb(Color pixel_color, int samples_per_pixel) {
    // Divide the color by the number of samples and gamma-correct for
    // gamma=2.0.
    Vec<int, 3> converted_color = pixel_color.map<int>([&](double val) {
        return static_cast<int>(
            256 * Math::clamp(sqrt(val / samples_per_pixel), 0.0, 0.999));
    });
    return converted_color;
}