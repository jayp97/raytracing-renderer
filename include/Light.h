#ifndef LIGHT_H
#define LIGHT_H

#include "Vector3.h"

class Light
{
public:
    // Constructor to initialize position and intensity
    Light(const Vector3 &position, const Vector3 &intensity);

    // Getters
    Vector3 getPosition() const { return position; }
    Vector3 getIntensity() const { return intensity; }

private:
    Vector3 position;  // Position of the light in the scene
    Vector3 intensity; // Intensity (color and brightness) of the light
};

#endif // LIGHT_H
