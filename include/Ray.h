#ifndef RAY_H
#define RAY_H

#include "Vector3.h"

class Ray
{
public:
    Vector3 origin;    // Starting point of the ray
    Vector3 direction; // Direction of the ray

    // Constructor
    Ray(const Vector3 &origin, const Vector3 &direction);

    // Method to compute a point along the ray at a given distance (t)
    Vector3 at(float t) const;
};

#endif