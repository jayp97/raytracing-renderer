#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Vector3.h"
#include "Ray.h"

class Triangle
{
public:
    Vector3 v0, v1, v2; // Triangle vertices

    // Constructor
    Triangle(const Vector3 &vertex0, const Vector3 &vertex1, const Vector3 &vertex2)
        : v0(vertex0), v1(vertex1), v2(vertex2) {}

    // Intersection method using Möller-Trumbore algoritm
    bool intersect(const Ray &ray, float &t, float &u, float &v) const;
};

#endif