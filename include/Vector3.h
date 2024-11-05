#ifndef VECTOR3_H
#define VECTOR3_H

#include <iostream>
#include <cmath>

class Vector3
{
public:
    float x, y, z;

    // Constructors
    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    // Basic Operations
    Vector3 operator+(const Vector3 &other) const;
    Vector3 operator-(const Vector3 &other) const;
    Vector3 operator*(float scalar) const;
    Vector3 operator/(float scalar) const;

    // Dot Product and Cross Product
    float dot(const Vector3 &other) const;
    Vector3 cross(const Vector3 &other) const;

    // Normalize vector
    Vector3 normalise() const;

    // Equality operator with tolerance for floating-point comparisons
    bool operator==(const Vector3 &other) const;

    // Stream insertion operator for printing
    friend std::ostream &operator<<(std::ostream &os, const Vector3 &v);

    // Debugging
    void print() const;
};

#endif
