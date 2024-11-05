#include "Vector3.h"
#include <cmath> // for std::fabs

// Addition
Vector3 Vector3::operator+(const Vector3 &other) const
{
    return Vector3(x + other.x, y + other.y, z + other.z);
}

// Subtraction
Vector3 Vector3::operator-(const Vector3 &other) const
{
    return Vector3(x - other.x, y - other.y, z - other.z);
}

// Equality operator with tolerance for floating-point precision
bool Vector3::operator==(const Vector3 &other) const
{
    const float tolerance = 1e-5;
    return std::fabs(x - other.x) < tolerance &&
           std::fabs(y - other.y) < tolerance &&
           std::fabs(z - other.z) < tolerance;
}

// Scalar multiplication
Vector3 Vector3::operator*(float scalar) const
{
    return Vector3(x * scalar, y * scalar, z * scalar);
}

// Scalar division
Vector3 Vector3::operator/(float scalar) const
{
    return Vector3(x / scalar, y / scalar, z / scalar);
}

// Cross product
Vector3 Vector3::cross(const Vector3 &other) const
{
    return Vector3(
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x);
}

// Normalise
Vector3 Vector3::normalise() const
{
    float length = std::sqrt(x * x + y * y + z * z);
    return (*this) / length;
}

// Dot product
float Vector3::dot(const Vector3 &other) const
{ // Add this implementation
    return x * other.x + y * other.y + z * other.z;
}

// Debug print method
void Vector3::print() const
{
    std::cout << "Vector3(" << x << ", " << y << ", " << z << ")" << std::endl;
}

// Stream insertion operator for outputting to std::ostream
std::ostream &operator<<(std::ostream &os, const Vector3 &v)
{
    os << "Vector3(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}
