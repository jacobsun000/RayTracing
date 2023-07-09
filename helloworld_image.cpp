#include <fstream>

int main(int argc, char const *argv[]) {
    std::ofstream outfile("helloworld.ppm");
    const int width = 512, height = 512;
    // PPM file header
    outfile << "P3" << std::endl
            << width << ' ' << height << std::endl
            << 255 << std::endl;
    for (int j = height - 1; j >= 0; --j) {
        for (int i = 0; i < width; ++i) {
            double r = double(i) / (width - 1);
            double g = double(j) / (height - 1);
            double b = 0.25;

            int ir = static_cast<int>(255.999 * r);
            int ig = static_cast<int>(255.999 * g);
            int ib = static_cast<int>(255.999 * b);

            outfile << ir << ' ' << ig << ' ' << ib << std::endl;
        }
    }
    return 0;
}
