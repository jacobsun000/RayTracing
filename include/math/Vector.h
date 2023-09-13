#pragma once
#include <array>
#include <functional>
#include <initializer_list>
#include <iostream>

#include "MathUtils.h"

template <typename T, size_t N>
class Vec {
    public:
    Vec() { _data.fill(T()); }

    // Fill length N vector with value of type T
    Vec(T value) { _data.fill(value); }

    // Fill length N vector with a list
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

    // Magnitude of vector
    T length() const { return sqrt(length_squared()); }

    // Square length of vector
    T length_squared() const {
        T result = 0;
        for (const auto& value : _data) {
            result += value * value;
        }
        return result;
    }

    // Return a new unit vector of current vector
    Vec<T, N> unit_vector() const { return (*this) / length(); }

    // Return true if the vector is close to zero in all dimensions.
    bool near_zero() const {
        const auto s = 1e-8;
        return (fabs(_data[0]) < s) && (fabs(_data[1]) < s) && (fabs(_data[2]) < s);
    }

    // Dot product of vector with another vector
    T dot(const Vec<T, N>& other) const {
        T result = 0;
        for (size_t i = 0; i < N; ++i) {
            result += _data[i] * other[i];
        }
        return result;
    }

    // Cross product of vector with another vector.
    // Only availiable in Vec<T, 3>
    Vec<T, N> cross(const Vec<T, N>& other) const {
        static_assert(
            N == 3, "Cross product is only defined for 3-dimensional vectors.");
        Vec<T, N> result;
        result[0] = (*this)[1] * other[2] - (*this)[2] * other[1];
        result[1] = (*this)[2] * other[0] - (*this)[0] * other[2];
        result[2] = (*this)[0] * other[1] - (*this)[1] * other[0];
        return result;
    }

    // Map each component of a vector to a new vector using func
    template <typename U = T>
    Vec<U, N> map(std::function<U(T)> func) const {
        Vec<U, N> result;
        for (size_t i = 0; i < N; i++) {
            result[i] = func(_data[i]);
        }
        return result;
    }

    // Map each component of a vector to a new vector using func
    template <typename U = T>
    Vec<U, N> map(std::function<U(T, size_t)> func) const {
        Vec<U, N> result;
        for (size_t i = 0; i < N; i++) {
            result[i] = func(_data[i], i);
        }
        return result;
    }

    // Apply func to each component of a vector
    Vec<T, N>& apply(std::function<void(T&)> func) {
        for (T& value : _data) {
            func(value);
        }
        return *this;
    }

    // Apply func to each component of a vector
    Vec<T, N>& apply(std::function<void(T&, size_t)> func) {
        for (size_t i = 0; i < N; i++) {
            func(_data[i], i);
        }
        return *this;
    }

    T& operator[](size_t index) { return _data[index]; }

    const T& operator[](size_t index) const { return _data[index]; }

    Vec<T, N> operator-() const {
        return map<T>([](T val) {
            return -val;
        });
    }

    Vec<T, N> operator+(const Vec<T, N>& other) const {
        return map<T>([&](T val, size_t i) {
            return val + other[i];
        });
    }

    Vec<T, N> operator-(const Vec<T, N>& other) const {
        return map<T>([&](T val, size_t i) {
            return val - other[i];
        });
    }

    Vec<T, N> operator*(const Vec<T, N>& other) const {
        return map<T>([&](T val, size_t i) {
            return val * other[i];
        });
    }

    Vec<T, N> operator*(const T& scalar) const {
        return map<T>([&](T val) {
            return val * scalar;
        });
    }

    Vec<T, N> operator/(const T& scalar) const {
        return map<T>([&](T val) {
            return val / scalar;
        });
    }

    Vec<T, N>& operator+=(const Vec<T, N>& other) {
        return apply([&](T& val, size_t i) {
            val += other[i];
        });
    }

    Vec<T, N>& operator-=(const Vec<T, N>& other) {
        return apply([&](T& val, size_t i) {
            val -= other[i];
        });
    }

    Vec<T, N>& operator*=(const Vec<T, N>& other) {
        return apply([&](T& val, size_t i) {
            val *= other[i];
        });
    }

    Vec<T, N>& operator/=(const Vec<T, N>& other) {
        return apply([&](T& val, size_t i) {
            val /= other[i];
        });
    }

    friend std::ostream& operator<<(std::ostream& os, const Vec<T, N>& vec) {
        os << "[ ";
        os << vec[0];
        for (std::size_t i = 1; i < N; i++) {
            os << ", " << vec[i];
        }
        os << " ]";
        return os;
    }

    friend Vec<T, N> operator*(const T& scalar, const Vec<T, N>& vec) {
        return vec * scalar;
    }

    static Vec<T, N> random(double min, double max) {
        auto rand = [min, max]() {
            return Math::random_double(min, max);
        };
        return { rand(), rand(), rand() };
    }

    static Vec<T, N> random() {
        auto rand = []() {
            return Math::random_double();
        };
        return { rand(), rand(), rand() };
    }

    private:
    std::array<T, N> _data;
};

using Vec3d = Vec<double, 3>;
using Point3d = Vec3d;

namespace Math {
Vec3d random_in_unit_sphere() {
    Vec3d p;
    do {
        p = Vec3d::random(-1, 1);
    } while (p.length_squared() >= 1);
    return p;
}

Vec3d random_in_unit_disk() {
    Vec3d p;
    do {
        p = Vec3d{Math::random_double(), Math::random_double(), 0};
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