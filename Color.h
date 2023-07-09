#pragma once

#include <fstream>

#include "Vector.h"

using Color = Vec3d;

std::ofstream& write_color(std::ofstream& ofs, const Color& color) {
    ofs << static_cast<int>(255.999 * color.x()) << ' '
        << static_cast<int>(255.999 * color.y()) << ' '
        << static_cast<int>(255.999 * color.z()) << std::endl;
    return ofs;
}