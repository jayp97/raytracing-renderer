#include "Ray.h"

// Constructor to initialise the ray with an origin and a direction
Ray::Ray(const Vector3 &origin, const Vector3 &direction) : origin(origin), direction(direction.normalise()) {}

// Method to get point along the ray: origin + t * direction
Vector3 Ray::at(float t) const
{
    return origin + direction * t;
};