#ifndef CYLINDER_H
#define CYLINDER_H

#include "Vector3.h" // Assuming a custom Vector3 class for 3D vectors
#include "Ray.h"     // Assuming a Ray class is available

class Cylinder
{
public:
    Vector3 center; // Center point of the cylinder
    Vector3 axis;   // Axis vector of the cylinder
    float radius;   // Radius of the cylinder
    float height;   // Height of the cylinder

    Cylinder(const Vector3 &center, const Vector3 &axis, float radius, float height);

    // Method to check ray-cylinder intersection
    bool intersect(const Ray &ray, float &t) const;

    // Method to get the normal at a point on the cylinder
    Vector3 getNormal(const Vector3 &point) const;
};

#endif
