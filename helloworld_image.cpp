#include <fstream>
#include <iomanip>
#include <iostream>

#include "Color.h"
#include "Vector.h"

int main(int argc, char const *argv[]) {
    std::ofstream outfile("helloworld.ppm");
    const int width = 512, height = 512;
    // PPM file header
    outfile << "P3" << std::endl
            << width << ' ' << height << std::endl
            << 255 << std::endl;
    for (int j = height - 1; j >= 0; --j) {
        for (int i = 0; i < width; ++i) {
            std::cout << "Rendering progress: " << std::setprecision(4)
                      << (double)(height - j) / (height)*100 << '%'
                      << std::endl;
            Color color{double(i) / (width - 1), double(j) / (height - 1),
                        0.25};
            write_color(outfile, color);
        }
    }
    std::cout << "Done" << std::endl;
    return 0;
}
