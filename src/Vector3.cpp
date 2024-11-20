// Vector3.cpp
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

// Compound Assignment Operators
Vector3 &Vector3::operator+=(const Vector3 &other)
{
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

Vector3 &Vector3::operator-=(const Vector3 &other)
{
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

Vector3 &Vector3::operator*=(float scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

Vector3 &Vector3::operator/=(float scalar)
{
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
}

// Unary Minus Operator
Vector3 Vector3::operator-() const
{
    return Vector3(-x, -y, -z);
}

// Dot product
float Vector3::dot(const Vector3 &other) const
{
    return x * other.x + y * other.y + z * other.z;
}

// Cross product
Vector3 Vector3::cross(const Vector3 &other) const
{
    return Vector3(
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x);
}

// Normalize
Vector3 Vector3::normalise() const
{
    float len = length();
    if (len == 0.0f)
        return Vector3(0.0f, 0.0f, 0.0f); // Prevent division by zero
    return (*this) / len;
}

// Length
float Vector3::length() const
{
    return std::sqrt(x * x + y * y + z * z);
}

// Length Squared
float Vector3::lengthSquared() const
{
    return x * x + y * y + z * z;
}

// Clamp
Vector3 Vector3::clamp(float min_val, float max_val) const
{
    float clampedX = std::fmax(min_val, std::fmin(x, max_val));
    float clampedY = std::fmax(min_val, std::fmin(y, max_val));
    float clampedZ = std::fmax(min_val, std::fmin(z, max_val));
    return Vector3(clampedX, clampedY, clampedZ);
}

// Equality operator with tolerance for floating-point precision
bool Vector3::operator==(const Vector3 &other) const
{
    const float tolerance = 1e-5f;
    return (std::fabs(x - other.x) < tolerance) &&
           (std::fabs(y - other.y) < tolerance) &&
           (std::fabs(z - other.z) < tolerance);
}

// Inequality operator
bool Vector3::operator!=(const Vector3 &other) const
{
    return !(*this == other);
}

// Element-wise multiplication
Vector3 Vector3::operator*(const Vector3 &other) const
{
    return Vector3(x * other.x, y * other.y, z * other.z);
}

// Reflect
Vector3 Vector3::reflect(const Vector3 &normal) const
{
    return *this - normal * (2.0f * this->dot(normal));
}

// Refraction
Vector3 Vector3::refract(const Vector3 &normal, float eta) const
{
    // Snell's Law: eta = n_i / n_t
    float cosi = std::max(-1.0f, std::min(1.0f, this->dot(normal)));
    float etai = 1.0f, etat = eta;
    Vector3 n = normal;
    if (cosi < 0)
    {
        cosi = -cosi;
    }
    else
    {
        std::swap(etai, etat);
        n = -normal;
    }
    float eta_ratio = etai / etat;
    float k = 1.0f - eta_ratio * eta_ratio * (1.0f - cosi * cosi);
    if (k < 0)
        return Vector3(0.0f, 0.0f, 0.0f); // Total internal reflection
    else
        return (*this) * eta_ratio + n * (eta_ratio * cosi - std::sqrt(k));
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
