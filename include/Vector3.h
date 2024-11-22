// Vector3.h
#ifndef VECTOR3_H
#define VECTOR3_H

#include <iostream>
#include <cmath>
#include <stdexcept>

class Vector3
{
public:
    float x, y, z;

    // Constructors
    Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vector3(float x_val, float y_val, float z_val) : x(x_val), y(y_val), z(z_val) {}

    // Basic Operations
    Vector3 operator+(const Vector3 &other) const;
    Vector3 operator-(const Vector3 &other) const;
    Vector3 operator*(float scalar) const;
    Vector3 operator/(float scalar) const;

    // Compound Assignment Operators
    Vector3 &operator+=(const Vector3 &other);
    Vector3 &operator-=(const Vector3 &other);
    Vector3 &operator*=(float scalar);
    Vector3 &operator/=(float scalar);

    // Operator for element-wise multiplication
    Vector3 operator*(const Vector3 &other) const;

    // Unary Minus Operator
    Vector3 operator-() const;

    // Dot Product and Cross Product
    float dot(const Vector3 &other) const;
    Vector3 cross(const Vector3 &other) const;

    // Normalize vector
    Vector3 normalise() const;

    // Add operator[] for component access
    float &operator[](int index);
    const float &operator[](int index) const;

    // Static methods for min and max
    static Vector3 min(const Vector3 &a, const Vector3 &b);
    static Vector3 max(const Vector3 &a, const Vector3 &b);

    Vector3 reflect(const Vector3 &normal) const;
    Vector3 refract(const Vector3 &normal, float eta) const;

    // Length and Length Squared
    float length() const;
    float lengthSquared() const;

    // Clamp vector components between min and max
    Vector3 clamp(float min_val, float max_val) const;

    // Equality operators with tolerance for floating-point comparisons
    bool operator==(const Vector3 &other) const;
    bool operator!=(const Vector3 &other) const;

    // Stream insertion operator for printing
    friend std::ostream &operator<<(std::ostream &os, const Vector3 &v);

    // Debugging
    void print() const;
};

// Scalar * Vector3 multiplication (global operator overload)
inline Vector3 operator*(float scalar, const Vector3 &v)
{
    return Vector3(v.x * scalar, v.y * scalar, v.z * scalar);
}

#endif // VECTOR3_H
