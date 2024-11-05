#ifndef SPHERE_H
#define SPHERE_H

#include "Vector3.h"
#include "Ray.h"

class Sphere
{
public:
    Vector3 center; // Center of Sphere
    float radius;   // Radius of Sphere

    // Constructor
    Sphere(const Vector3 &c, float r) : center(c), radius(r) {}

    // Intersection method that returns true if the ray intersects the sphere
    bool intersect(const Ray &ray, float &t) const;
};

#endif