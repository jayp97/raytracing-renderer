#ifndef COLOR_H
#define COLOR_H

#include <algorithm> // For std::min and std::max
#include <iostream>
#include <cmath> // For std::fabs

struct Color
{
    float r, g, b;

    // Constructors
    Color() : r(0.0f), g(0.0f), b(0.0f) {}
    Color(float red, float green, float blue) : r(red), g(green), b(blue) {}

    // Operator Overloads
    Color operator+(const Color &other) const
    {
        return Color(r + other.r, g + other.g, b + other.b);
    }

    Color &operator+=(const Color &other)
    {
        r += other.r;
        g += other.g;
        b += other.b;
        return *this;
    }

    Color operator*(float scalar) const
    {
        return Color(r * scalar, g * scalar, b * scalar);
    }

    Color operator*(const Color &other) const // Element-wise multiplication
    {
        return Color(r * other.r, g * other.g, b * other.b);
    }

    Color clamp(float minVal, float maxVal) const
    {
        return Color(
            std::max(minVal, std::min(r, maxVal)),
            std::max(minVal, std::min(g, maxVal)),
            std::max(minVal, std::min(b, maxVal)));
    }

    // Tone mapping method with exposure
    Color toneMap(float exposure) const;

    bool operator==(const Color &other) const
    {
        const float tolerance = 1e-5f;
        return (std::fabs(r - other.r) < tolerance) &&
               (std::fabs(g - other.g) < tolerance) &&
               (std::fabs(b - other.b) < tolerance);
    }

    bool operator!=(const Color &other) const
    {
        return !(*this == other);
    }

    // Stream insertion operator for printing
    friend std::ostream &operator<<(std::ostream &os, const Color &c)
    {
        os << "Color(" << c.r << ", " << c.g << ", " << c.b << ")";
        return os;
    }
};

#endif // COLOR_H
