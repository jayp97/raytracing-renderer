#include "Vector3.h"

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

// Boolean
bool Vector3::operator==(const Vector3 &other) const
{
    return x == other.x && y == other.y && z == other.z;
}

// Scalar multiplication
Vector3 Vector3::operator*(float scalar) const
{
    return Vector3(x * scalar, y * scalar, z * scalar);
}

// Scalar Division
Vector3 Vector3::operator/(float scalar) const
{
    return Vector3(x / scalar, y / scalar, z / scalar);
}

// Dot Product
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

// Debug print
void Vector3::print() const
{
    std::cout << "Vector3(" << x << ", " << y << ", " << z << ")" << std::endl;
}