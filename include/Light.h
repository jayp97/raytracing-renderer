#ifndef LIGHT_H
#define LIGHT_H

#include "Vector3.h"
#include "Color.h"

class Light
{
public:
    // Constructor to initialize position and intensity
    Light(const Vector3 &position, const Color &intensity);

    // Getters
    Vector3 getPosition() const { return position; }
    Color getIntensity() const { return intensity; }

private:
    Vector3 position; // Position of the light in the scene
    Color intensity;  // Intensity (color and brightness) of the light
};

#endif // LIGHT_H
