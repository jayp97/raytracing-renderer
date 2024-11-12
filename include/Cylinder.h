// Cylinder.h
#ifndef CYLINDER_H
#define CYLINDER_H

#include "Vector3.h"      // Custom Vector3 class for 3D vectors
#include "Ray.h"          // Ray class
#include "Material.h"     // Material class
#include "Intersection.h" // Intersection structure
#include "Object.h"       // Base class for objects

class Cylinder : public Object
{
public:
    // Constructor with material
    Cylinder(const Vector3 &center, const Vector3 &axis, float radius, float height, const Material &material);

    // Method to check ray-cylinder intersection and populate Intersection
    bool intersect(const Ray &ray, Intersection &hit) const override;

private:
    // Method to get the normal at a point on the cylinder
    Vector3 getNormal(const Vector3 &point) const;

    // Attributes
    Vector3 center;    // Center point of the cylinder (base)
    Vector3 axis;      // Normalized axis vector of the cylinder
    float radius;      // Radius of the cylinder
    float height;      // Height of the cylinder
    Material material; // Material property for rendering
};

#endif // CYLINDER_H
