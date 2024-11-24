// Color.h
#ifndef COLOR_H
#define COLOR_H

#include <algorithm>
#include <iostream>
#include <cmath>
#include <stdexcept>

struct Color
{
    float r, g, b;

    // Constructors
    Color() : r(0.0f), g(0.0f), b(0.0f) {}
    Color(float red, float green, float blue) : r(red), g(green), b(blue) {}

    // Operator overloads
    Color operator+(const Color &other) const;
    Color &operator+=(const Color &other);
    Color operator-(const Color &other) const; // Added operator-
    Color operator*(float scalar) const;
    friend Color operator*(float scalar, const Color &color); // Added operator* for float * Color
    Color &operator*=(float scalar);
    Color operator*(const Color &other) const;
    Color operator/(float scalar) const;
    Color operator/(const Color &other) const;

    // Compound assignment operator for division by float
    Color &operator/=(float scalar);

    // Clamp method
    Color clamp(float minVal, float maxVal) const;

    // Gamma correction
    Color gammaCorrect(float gamma) const;

    // Method to get the maximum component
    float maxComponent() const;
};

// Addition
inline Color Color::operator+(const Color &other) const
{
    return Color(r + other.r, g + other.g, b + other.b);
}

inline Color &Color::operator+=(const Color &other)
{
    r += other.r;
    g += other.g;
    b += other.b;
    return *this;
}

// Subtraction
inline Color Color::operator-(const Color &other) const
{
    return Color(r - other.r, g - other.g, b - other.b);
}

// Scalar multiplication
inline Color Color::operator*(float scalar) const
{
    return Color(r * scalar, g * scalar, b * scalar);
}

inline Color &Color::operator*=(float scalar)
{
    r *= scalar;
    g *= scalar;
    b *= scalar;
    return *this;
}

// Element-wise multiplication
inline Color Color::operator*(const Color &other) const
{
    return Color(r * other.r, g * other.g, b * other.b);
}

// Scalar division
inline Color Color::operator/(float scalar) const
{
    if (scalar == 0.0f)
        throw std::runtime_error("Division by zero in Color::operator/");

    return Color(r / scalar, g / scalar, b / scalar);
}

// Element-wise division
inline Color Color::operator/(const Color &other) const
{
    return Color(r / other.r, g / other.g, b / other.b);
}

// Compound assignment operator for division by float
inline Color &Color::operator/=(float scalar)
{
    if (scalar == 0.0f)
        throw std::runtime_error("Division by zero in Color::operator/=");

    r /= scalar;
    g /= scalar;
    b /= scalar;
    return *this;
}

// Clamp color components between min and max
inline Color Color::clamp(float minVal, float maxVal) const
{
    return Color(
        std::max(minVal, std::min(r, maxVal)),
        std::max(minVal, std::min(g, maxVal)),
        std::max(minVal, std::min(b, maxVal)));
}

// Gamma correction
inline Color Color::gammaCorrect(float gamma) const
{
    float invGamma = 1.0f / gamma;
    return Color(
        std::pow(std::max(r, 0.0f), invGamma),
        std::pow(std::max(g, 0.0f), invGamma),
        std::pow(std::max(b, 0.0f), invGamma));
}

// Method to get the maximum component
inline float Color::maxComponent() const
{
    return std::max(r, std::max(g, b));
}

// Scalar multiplication from the left (float * Color)
inline Color operator*(float scalar, const Color &color)
{
    return Color(color.r * scalar, color.g * scalar, color.b * scalar);
}

#endif // COLOR_H
