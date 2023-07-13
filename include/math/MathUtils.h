#pragma once

#include <cmath>
#include <limits>
#include <random>

#include "Vector.h"

namespace Math {

const double INF = std::numeric_limits<double>::infinity();
const double PI = 3.1415926535897932385;

inline double deg_to_rad(double degrees) { return degrees * PI / 180.0; }

inline double rad_to_deg(double radians) { return radians / PI * 180.0; }

inline double random_double() {
    // Returns a random real in [0,1).
    thread_local std::minstd_rand generator(std::random_device{}());
    thread_local std::uniform_real_distribution<double> distribution(0.0, 1.0);

    return distribution(generator);
}

inline double random_double(double min, double max) {
    // Returns a random real in [min,max).
    static std::uniform_real_distribution<double> distribution(min, max);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

};  // namespace Math