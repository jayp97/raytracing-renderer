#ifndef LIGHT_H
#define LIGHT_H

#include "Vector3.h"
#include "Color.h"
#include <random>

enum class LightType
{
    Point,
    Area
};

class Light
{
public:
    // Constructor for point light
    Light(const Vector3 &position, const Color &intensity);

    // Constructor for area light (spherical)
    Light(const Vector3 &position, const Color &intensity, float areaRadius);

    // Getters
    Vector3 getPosition() const { return position; }
    Color getIntensity() const { return intensity; }
    LightType getType() const { return type; }
    float getAreaRadius() const { return areaRadius; }

    // Sample a point on the area light's surface (only for area lights)
    Vector3 samplePosition(std::mt19937 &rng, std::uniform_real_distribution<float> &dist) const;

private:
    Vector3 position; // Position of the light in the scene
    Color intensity;  // Intensity (color and brightness) of the light
    LightType type;   // Type of the light (Point or Area)
    float areaRadius; // Radius of the area light (applicable for area lights)
};

#endif // LIGHT_H
