#ifndef SPHERE_H
#define SPHERE_H

#include "Vector3.h"
#include "Ray.h"
#include "Material.h" // Include Material

class Sphere
{
public:
    // Constructor with material
    Sphere(const Vector3 &c, float r, const Material &m) : center(c), radius(r), material(m) {}

    // Intersection method that returns true if the ray intersects the sphere
    bool intersect(const Ray &ray, float &t) const;

    // Attributes
    Vector3 center;    // Center of Sphere
    float radius;      // Radius of Sphere
    Material material; // Material of Sphere
};

#endif // SPHERE_H
