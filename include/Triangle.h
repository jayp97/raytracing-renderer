#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Vector3.h"
#include "Ray.h"
#include "Material.h" // Include Material

class Triangle
{
public:
    // Constructor with material
    Triangle(const Vector3 &vertex0, const Vector3 &vertex1, const Vector3 &vertex2, const Material &m)
        : v0(vertex0), v1(vertex1), v2(vertex2), material(m) {}

    // Intersection method using Möller-Trumbore algorithm
    bool intersect(const Ray &ray, float &t, float &u, float &v) const;

    // Triangle vertices
    Vector3 v0, v1, v2;

    // Material property for rendering
    Material material;
};

#endif // TRIANGLE_H
