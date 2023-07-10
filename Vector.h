#pragma once

#include <array>
#include <initializer_list>
#include <iostream>

#include "Math.h"

template <typename T, size_t N>
class Vec {
   public:
    Vec() { _data.fill(T()); }

    Vec(T value) { _data.fill(value); }

    Vec(std::initializer_list<T> values) {
        if (values.size() != N) {
            throw std::runtime_error("Invalid initializer list size");
        }

        size_t i = 0;
        for (const auto& value : values) {
            _data[i++] = value;
        }
    }

    size_t size() const { return N; }

    T x() const {
        static_assert(N >= 1, "x component is not available.");
        return _data[0];
    }

    T y() const {
        static_assert(N >= 2, "y component is not available.");
        return _data[1];
    }

    T z() const {
        static_assert(N >= 3, "z component is not available.");
        return _data[2];
    }

    T length() const { return sqrt(length_squared()); }

    T length_squared() const {
        T result = 0;
        for (const auto& value : _data) {
            result += value * value;
        }
        return result;
    }

    Vec<T, N> unit_vector() const { return (*this) / length(); }

    bool near_zero() const {
        // Return true if the vector is close to zero in all dimensions.
        const auto s = 1e-8;
        return (fabs(_data[0]) < s) && (fabs(_data[1]) < s) &&
               (fabs(_data[2]) < s);
    }

    T dot(const Vec<T, N>& other) const {
        T result = 0;
        for (size_t i = 0; i < N; ++i) {
            result += _data[i] * other[i];
        }
        return result;
    }

    Vec<T, N> cross(const Vec<T, N>& other) const {
        static_assert(
            N == 3, "Cross product is only defined for 3-dimensional vectors.");
        Vec<T, N> result;
        result[0] = (*this)[1] * other[2] - (*this)[2] * other[1];
        result[1] = (*this)[2] * other[0] - (*this)[0] * other[2];
        result[2] = (*this)[0] * other[1] - (*this)[1] * other[0];
        return result;
    }

    T& operator[](size_t index) { return _data[index]; }

    const T& operator[](size_t index) const { return _data[index]; }

    Vec<T, N> operator-() const {
        Vec<T, N> result;
        for (size_t i = 0; i < N; i++) {
            result[i] = -_data[i];
        }
        return result;
    }

    Vec<T, N> operator+(const Vec<T, N>& other) const {
        Vec<T, N> result;
        for (size_t i = 0; i < N; ++i) {
            result[i] = _data[i] + other[i];
        }
        return result;
    }

    Vec<T, N> operator-(const Vec<T, N>& other) const {
        Vec<T, N> result;
        for (size_t i = 0; i < N; ++i) {
            result[i] = _data[i] - other[i];
        }
        return result;
    }

    Vec<T, N> operator*(const Vec<T, N>& other) const {
        Vec<T, N> result;
        for (size_t i = 0; i < N; ++i) {
            result[i] = _data[i] * other[i];
        }
        return result;
    }

    Vec<T, N> operator*(const T& scalar) const {
        Vec<T, N> result;
        for (size_t i = 0; i < N; ++i) {
            result[i] = _data[i] * scalar;
        }
        return result;
    }

    Vec<T, N> operator/(const T& scalar) const {
        Vec<T, N> result;
        for (size_t i = 0; i < N; ++i) {
            result[i] = _data[i] / scalar;
        }
        return result;
    }

    Vec<T, N>& operator+=(const Vec<T, N>& other) {
        for (size_t i = 0; i < N; i++) {
            _data[i] += other[i];
        }
        return *this;
    }

    Vec<T, N>& operator-=(const Vec<T, N>& other) {
        for (size_t i = 0; i < N; i++) {
            _data[i] -= other[i];
        }
        return *this;
    }

    Vec<T, N>& operator*=(const Vec<T, N>& other) {
        for (size_t i = 0; i < N; i++) {
            _data[i] *= other[i];
        }
        return *this;
    }

    Vec<T, N>& operator/=(const Vec<T, N>& other) {
        for (size_t i = 0; i < N; i++) {
            _data[i] /= other[i];
        }
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const Vec<T, N>& vec) {
        os << "[ ";
        for (std::size_t i = 0; i < N; i++) {
            os << vec[i] << (i < N - 1 ? ", " : " ");
        }
        os << "]";
        return os;
    }

    friend Vec<T, N> operator*(const T& scalar, const Vec<T, N>& vec) {
        return vec * scalar;
    }

    static Vec<T, N> random(double min, double max) {
        auto rand = [&]() { return Math::random_double(min, max); };
        return {rand(), rand(), rand()};
    }

    static Vec<T, N> random() {
        auto rand = []() { return Math::random_double(); };
        return {rand(), rand(), rand()};
    }

   private:
    std::array<T, N> _data;
};

using Vec3d = Vec<double, 3>;
using Point3d = Vec3d;

namespace Math {
inline Vec3d random_in_unit_sphere() {
    Vec3d p;
    do {
        p = Vec3d::random(-1, 1);
    } while (p.length_squared() >= 1);
    return p;
}

Vec3d random_in_hemisphere(const Vec3d& normal) {
    Vec3d in_unit_sphere = random_in_unit_sphere();
    // In the same hemisphere as the normal
    if (in_unit_sphere.dot(normal) > 0.0)
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}

Vec3d random_unit_vector() { return random_in_unit_sphere().unit_vector(); }

}  // namespace Math