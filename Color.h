#pragma once

#include <fstream>
#include <sstream>
#include <string>

#include "Common.h"
#include "Math.h"
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

void write_color(std::ostream &out, Color pixel_color, int samples_per_pixel) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Divide the color by the number of samples and gamma-correct for
    // gamma=2.0.
    auto scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    auto convert_color = [](double val) {
        return static_cast<int>(256 * Math::clamp(val, 0.0, 0.999));
    };

    // Write the translated [0,255] value of each color component.
    out << convert_color(r) << ' ' << convert_color(g) << ' '
        << convert_color(b) << '\n';
}