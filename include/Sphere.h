// Sphere.h
#ifndef SPHERE_H
#define SPHERE_H

#include "Vector3.h"
#include "Ray.h"
#include "Material.h"
#include "Intersection.h" // Include Intersection
#include "Object.h"       // Assuming Sphere inherits from Object

class Sphere : public Object // Inheriting from a base Object class
{
public:
    // Constructor with material
    Sphere(const Vector3 &c, float r, const Material &m)
        : center(c), radius(r), material(m) {}

    // Intersection method that fills in the Intersection data if there's an intersection
    bool intersect(const Ray &ray, Intersection &hit) const override;

    // Attributes
    Vector3 center;    // Center of Sphere
    float radius;      // Radius of Sphere
    Material material; // Material of Sphere
};

#endif // SPHERE_H
