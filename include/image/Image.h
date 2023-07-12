#pragma once

#include <fstream>
#include <iostream>
#include <string>

#include "Common.h"
#include "Pixel.h"

struct ImageOption {
    int width;
    int height;
};

class Image {
   public:
    int width;
    int height;
    vector<vector<Pixel>> data;

   protected:
    ImageOption _option;

   public:
    Image(ImageOption option) : width(option.width), height(option.height) {
        data.resize(height, vector<Pixel>(width));
    }
    virtual void write() {}
};

class PPM_Image : public Image {
   public:
    PPM_Image(ImageOption option, const std::string& filename)
        : Image(option), _filename(filename) {}
    void write() override {
        std::ofstream outfile(_filename);
        outfile << "P3\n" << width << ' ' << height << '\n' << 255 << '\n';
        for (const auto& row : data) {
            for (const Pixel& pixel : row) {
                outfile << pixel.x() << ' ' << pixel.y() << ' ' << pixel.z()
                        << '\n';
            }
        }
    }

   private:
    std::string _filename;
};