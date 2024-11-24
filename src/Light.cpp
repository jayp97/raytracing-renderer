#include "Light.h"
#include <cmath>

// Constructor for point light
Light::Light(const Vector3 &position, const Color &intensity)
    : position(position), intensity(intensity), type(LightType::Point), areaRadius(0.0f) {}

// Constructor for area light (spherical)
Light::Light(const Vector3 &position, const Color &intensity, float areaRadius)
    : position(position), intensity(intensity), type(LightType::Area), areaRadius(areaRadius) {}

// Helper function to sample a random point on a spherical area light
Vector3 Light::samplePosition(std::mt19937 &rng, std::uniform_real_distribution<float> &dist) const
{
    if (type != LightType::Area)
    {
        return position; // For point lights, return the position itself
    }

    // Uniform sampling on a sphere
    float theta = 2.0f * M_PI * dist(rng);
    float phi = acos(1.0f - 2.0f * dist(rng));
    float x = areaRadius * sin(phi) * cos(theta);
    float y = areaRadius * sin(phi) * sin(theta);
    float z = areaRadius * cos(phi);

    // Assuming the area light is oriented upwards; adjust as needed
    // Translate the sampled point to the area light's position
    return Vector3(position.x + x, position.y + y, position.z + z);
}
