#include "Light.h"
#include "Color.h"

// Constructor implementation
Light::Light(const Vector3 &position, const Color &intensity)
    : position(position), intensity(intensity) {}
