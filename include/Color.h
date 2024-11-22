// Color.h
#ifndef COLOR_H
#define COLOR_H

#include <algorithm>
#include <iostream>
#include <cmath>

struct Color
{
    float r, g, b;

    // Constructors
    Color() : r(0.0f), g(0.0f), b(0.0f) {}
    Color(float red, float green, float blue) : r(red), g(green), b(blue) {}

    // Operator overloads
    Color operator+(const Color &other) const;
    Color &operator+=(const Color &other);
    Color operator*(float scalar) const;
    Color &operator*=(float scalar);
    Color operator*(const Color &other) const;

    // Clamp method
    Color clamp(float minVal, float maxVal) const;

    // Tone mapping using Filmic operator
    Color toneMap() const;

    // Gamma correction
    Color gammaCorrect(float gamma) const;

    // Adjust saturation
    Color adjustSaturation(float saturation) const;
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

// Clamp color components between min and max
inline Color Color::clamp(float minVal, float maxVal) const
{
    return Color(
        std::max(minVal, std::min(r, maxVal)),
        std::max(minVal, std::min(g, maxVal)),
        std::max(minVal, std::min(b, maxVal)));
}

// Filmic tone mapping operator (Uncharted 2)
inline Color Color::toneMap() const
{
    auto Uncharted2Tonemap = [](float x) -> float
    {
        const float A = 0.15f;
        const float B = 0.50f;
        const float C = 0.10f;
        const float D = 0.20f;
        const float E = 0.02f;
        const float F = 0.30f;
        return ((x * (A * x + C * B) + D * E) / (x * (A * x + B) + D * F)) - E / F;
    };

    return Color(
        Uncharted2Tonemap(r),
        Uncharted2Tonemap(g),
        Uncharted2Tonemap(b));
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

// Adjust saturation
inline Color Color::adjustSaturation(float saturation) const
{
    // Convert to grayscale using luminosity method
    float gray = 0.2126f * r + 0.7152f * g + 0.0722f * b;
    return Color(
        gray + (r - gray) * saturation,
        gray + (g - gray) * saturation,
        gray + (b - gray) * saturation);
}

#endif // COLOR_H
